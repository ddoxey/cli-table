#include <cmath>
#include <numeric>
#include <algorithm>
#include <boost/tokenizer.hpp>
#include <nlohmann/json.hpp>
#include "table.hpp"


#define MIN_PADDING 2
#define HORZ LIGHT_HORIZONTAL
#define VERT LIGHT_VERTICAL
#define TLEFT LIGHT_DOWN_AND_RIGHT
#define BLEFT LIGHT_UP_AND_RIGHT
#define TRIGHT LIGHT_DOWN_AND_LEFT
#define BRIGHT LIGHT_UP_AND_LEFT
#define TTEE LIGHT_DOWN_AND_HORIZONTAL
#define BTEE LIGHT_UP_AND_HORIZONTAL
#define CROSS LIGHT_VERTICAL_AND_HORIZONTAL
#define LTEE LIGHT_VERTICAL_AND_RIGHT
#define RTEE LIGHT_VERTICAL_AND_LEFT

using json = nlohmann::json;


Table::Table()
{
    rows = std::vector<std::shared_ptr<std::vector<std::shared_ptr<Table::Cell>>>>();
}

Table::Table(const size_t width)
    : minimum_table_width(width)
{
    rows = std::vector<std::shared_ptr<std::vector<std::shared_ptr<Table::Cell>>>>();
}

Table::Table(const std::string &title, const size_t width)
    : minimum_table_width(width)
{
    rows = std::vector<std::shared_ptr<std::vector<std::shared_ptr<Table::Cell>>>>();
    add_header(title);
}

Table::Table(const std::string &title)
{
    rows = std::vector<std::shared_ptr<std::vector<std::shared_ptr<Table::Cell>>>>();
    add_header(title);
}

Table::Table(std::istream &ifs)
{
    rows = std::vector<std::shared_ptr<std::vector<std::shared_ptr<Table::Cell>>>>();

    using tokenizer = boost::tokenizer<boost::escaped_list_separator<char>>;

    std::string line;

    while (std::getline(ifs, line))
    {
        tokenizer tokens{line};

        auto cols = std::shared_ptr<std::vector<std::shared_ptr<Table::Cell>>>(new std::vector<std::shared_ptr<Table::Cell>>());

        if (tokens.begin() == tokens.end())
        {
            cols.get()->push_back(std::shared_ptr<Table::Cell>(new Table::Cell("")));
        }
        else
        {
            for (const auto &token : tokens)
            {
                cols.get()->push_back(std::shared_ptr<Table::Cell>(new Table::Cell(token)));
            }
        }

        rows.push_back(cols);
    }
}

void Table::set_style(const json &config)
{
    style.update(config);
}

void Table::add_header(const std::string &title)
{
    auto header = std::shared_ptr<std::vector<std::shared_ptr<Table::Cell>>>(new std::vector<std::shared_ptr<Table::Cell>>());
    header.get()->push_back(std::shared_ptr<Table::Cell>(new Table::Cell(title)));
    rows.push_back(header);

    json header_style(R"(
        {
            "row": {
                "where": {
                    "n": 0
                },
                "align": "center"
            }
        }
    )"_json);

    header_style["row"]["where"]["n"] = rows.size() - 1;

    style.update(header_style);

    auto next_row = std::shared_ptr<std::vector<std::shared_ptr<Table::Cell>>>(new std::vector<std::shared_ptr<Table::Cell>>());
    rows.push_back(next_row);
}

void Table::add_row(const std::shared_ptr<std::vector<std::string>> &columns)
{
    auto cols = std::shared_ptr<std::vector<std::shared_ptr<Table::Cell>>>(new std::vector<std::shared_ptr<Table::Cell>>());

    std::for_each(columns.get()->begin(), columns.get()->end(),
        [&cols](std::string &col)
        {
            cols.get()->push_back(std::shared_ptr<Table::Cell>(new Table::Cell(col)));
        }
    );

    rows.push_back(cols);
}

void Table::add_col(const std::string &column)
{
    if (column == "\n" || rows.size() == 0)
    {
        auto cols = std::shared_ptr<std::vector<std::shared_ptr<Table::Cell>>>(new std::vector<std::shared_ptr<Table::Cell>>());

        rows.push_back(cols);

        if (column == "\n")
            return;
    }

    rows.back().get()->push_back(std::shared_ptr<Table::Cell>(new Table::Cell(column)));
}

void Table::horizontal(std::ostream &out, std::vector<std::shared_ptr<std::vector<size_t>>> &width_for, size_t index) const
{
    auto left  = index == 0 ? TLEFT  : BLEFT;
    auto right = index == 0 ? TRIGHT : BRIGHT;
    auto tee   = index == 0 ? TTEE   : BTEE;

    index = index == 0 ? index : width_for.size() - 1;

    std::for_each(
        width_for.at(index).get()->begin(),
        width_for.at(index).get()->end(),
        [&out, &left, &tee](size_t width)
    {
        out << left;

        for (size_t i = 0; i < width; i++)
            out << HORZ;

        left = tee;
    });

    out << right
        << std::endl;
}

void Table::intermediate(std::ostream &out, std::vector<std::shared_ptr<std::vector<size_t>>> &width_for, size_t index) const
{
    if (index >= width_for.size() - 1)
        return;

    size_t upper_index = index;
    size_t below_index = index + 1;

    size_t upper_col_n = width_for.at(upper_index).get()->size();
    size_t below_col_n = width_for.at(below_index).get()->size();

    if (upper_col_n == below_col_n)
        return;

    auto upper_vert = std::vector<bool>();
    upper_vert.push_back(true);

    std::for_each(
        width_for.at(upper_index).get()->begin(),
        width_for.at(upper_index).get()->end(),
        [&upper_vert](size_t width)
    {
        for (size_t i = 0; i < width; i++)
            upper_vert.push_back(false);

        upper_vert.push_back(true);
    });

    auto below_vert = std::vector<bool>();
    below_vert.push_back(true);

    std::for_each(
        width_for.at(below_index).get()->begin(),
        width_for.at(below_index).get()->end(),
        [&below_vert](size_t width)
    {
        for (size_t i = 0; i < width; i++)
            below_vert.push_back(false);

        below_vert.push_back(true);
    });

    out << LTEE;

    auto tee = CROSS;

    for (size_t c = 1; c < upper_vert.size() - 1; c++)
    {
        tee =  upper_vert[c] &&  below_vert[c] ? CROSS
            :  upper_vert[c] && !below_vert[c] ? BTEE
            : !upper_vert[c] &&  below_vert[c] ? TTEE
            :                                    HORZ;

        out << tee;
    }

    out << RTEE
        << std::endl;
}

std::ostream& Table::render(std::ostream &out) const
{
    if (rows.size() == 0)
        return out;

    size_t row_n = 0, col_n = 0;

    auto width_for = compute_widths_();

    horizontal(out, width_for, 0);

    std::for_each(
        rows.begin(),
        rows.end(),
        [&out, &row_n, &col_n, &width_for, this](auto &cols)
    {
        if (cols.get()->begin() ==  cols.get()->end())
            return;

        size_t col_count = width_for.at(row_n).get()->size();

        auto row_style = style.row(row_n, cols);

        col_n = 0;

        std::for_each(
            cols.get()->begin(),
            cols.get()->end(),
            [&out, &row_n, &col_n, &width_for, &col_count, &row_style, this](auto &col)
        {
            auto col_style = style.col(col_n, col);
            auto position_style = style.position(row_n, col_n, col);

            col.get()->set_style(row_style, col_style, position_style);

            auto sgr = col.get()->sgr_codes();
            Align align = col.get()->alignment();

            size_t width = width_for.at(row_n).get()->at(col_n);
            size_t actual_width = col.get()->length();
            size_t width_diff = width - actual_width;

            if (col_count == 1
                && std::get<1>(row_style) != automatic)
            {
                align = std::get<1>(row_style);
            }

            std::string pad_left(" ");
            std::string pad_right(" ");

            if (align == left)
            {
                size_t right_size = width - ( 1 + actual_width );
                pad_right.resize(right_size);
                std::fill(pad_right.begin(), pad_right.end(), ' ');
            }
            else if (align == center)
            {
                size_t left_size = width_diff / 2;
                size_t right_size = left_size + (width_diff % 2 ? 1 : 0);

                pad_left.resize(left_size);
                std::fill(pad_left.begin(), pad_left.end(), ' ');

                pad_right.resize(right_size);
                std::fill(pad_right.begin(), pad_right.end(), ' ');
            }
            else if (align == right)
            {
                size_t left_size = width - ( 1 + actual_width );
                pad_left.resize(left_size);
                std::fill(pad_left.begin(), pad_left.end(), ' ');
            }

            out << VERT
                << std::get<0>(sgr)
                << pad_left
                << *col.get()
                << pad_right
                << std::get<1>(sgr);

            col_n++;
        });

        out << VERT << std::endl;

        this->intermediate(out, width_for, row_n);

        row_n++;
    });

    horizontal(out, width_for, -1);

    return out;
}

std::vector<std::shared_ptr<std::vector<size_t>>> Table::compute_widths_() const
{
    auto width_for = std::vector<std::shared_ptr<std::vector<size_t>>>();

    size_t row_n = 0,
           col_n = 0,
           max_col_count = 0,
           row_width = 0,
           max_row_width = 0;

    std::for_each(
        rows.begin(),
        rows.end(),
        [&row_n, &col_n, &max_col_count, &row_width, &max_row_width, &width_for](auto &cols)
    {
        col_n = 0;
        row_width = 0;

        auto widths = std::shared_ptr<std::vector<size_t>>(new std::vector<size_t>());

        if (cols.get()->begin() ==  cols.get()->end())
            return;

        std::for_each(
            cols.get()->begin(),
            cols.get()->end(),
            [&row_n, &col_n, &row_width, &widths, &width_for](auto &col)
        {
            size_t above_width = 0;
            size_t width = MIN_PADDING + col.get()->length();

            for (int r_n = row_n - 1; r_n >= 0; r_n--)
            {
                if (width_for.at(r_n).get()->size() == 1
                    || width_for.at(r_n).get()->size() <= col_n)
                    continue;

                above_width = width_for.at(r_n).get()->at(col_n);
                break;
            }

            if (width <= above_width)
            {
                width = above_width;
            }
            else if (row_n > 0)
            {
                for (int r_n = row_n - 1; r_n >= 0; r_n--)
                {
                    if (width_for.at(r_n).get()->size() == 1
                        || width_for.at(r_n).get()->size() <= col_n)
                        continue;

                    // update rows above with new wider width
                    width_for.at(r_n).get()->at(col_n) = width;
                }
            }

            widths.get()->push_back(width);

            row_width += width;

            col_n++;
        });

        width_for.push_back(widths);

        size_t col_count = widths.get()->size();
        size_t markup_count = 1 + col_count;

        max_col_count = std::max(col_count, max_col_count);
        max_row_width = std::max(row_width + markup_count, max_row_width);

        row_n++;
    });

    max_row_width = std::max(max_row_width, minimum_table_width);

    std::for_each(width_for.begin(), width_for.end(), [&max_row_width](auto &cols)
    {
        size_t col_count = cols.get()->size();
        size_t markup_count = 1 + col_count;

        size_t col_n = 0;

        while (std::accumulate(cols.get()->begin(),
                               cols.get()->end(),
                               markup_count) < max_row_width)
        {
            // bulk up columns until each row is the same width
            cols.get()->at(col_n++) += 1;

            if (col_n == col_count) col_n = 0;
        }
    });

    return width_for;
}

std::ostream& operator << (std::ostream &out, const Table &t)
{
    return t.render(out);
}

Table::Cell::Cell(const std::string &data)
{
    text = data;
}

size_t Table::Cell::length() const
{
    return text.length();
}

void Table::Cell::set_style(
    const std::pair<std::vector<size_t>, Align> &row_style,
    const std::pair<std::vector<size_t>, Align> &col_style,
    const std::pair<std::vector<size_t>, Align> &position_style)
{
    auto row_sgr = std::get<0>(row_style);
    auto row_align = std::get<1>(row_style);
    auto col_sgr = std::get<0>(col_style);
    auto col_align = std::get<1>(col_style);
    auto position_sgr = std::get<0>(position_style);
    auto position_align = std::get<1>(position_style);

    if (row_sgr.size() > 0) sgr = row_sgr;
    if (col_sgr.size() > 0) sgr = col_sgr;
    if (position_sgr.size() > 0) sgr = position_sgr;

    align = row_align;
    if (col_align != automatic) align = col_align;
    if (position_align != automatic) align = position_align;
    if (align == automatic) align = left;
}

Align Table::Cell::alignment() const
{
    return align;
}

std::pair<std::string, std::string> Table::Cell::sgr_codes() const
{
    std::pair<std::string, std::string> codes;

    if (sgr.size() > 0)
    {
        std::string semicolon("");
        std::stringstream esc_seq;

        esc_seq << "\033[";

        for (const auto &code : sgr)
        {
            esc_seq << semicolon << code;

            semicolon = ";";
        }

        esc_seq << "m";

        codes = std::make_pair(esc_seq.str(), "\033[0m");
    }
    else
    {
        codes = std::make_pair("", "");
    }

    return codes;
}

bool Table::Cell::cmp(std::string &subtext) const
{
    return text.find(subtext) != std::string::npos;
}

std::ostream& Table::Cell::str(std::ostream &out) const
{
    return out << text;
}

std::ostream& operator << (std::ostream &out, const Table::Cell &c)
{
    return c.str(out);
}

void Table::Style::update(const json &config)
{
    if (config.count("row") != 0)
    {
        if (config["row"].is_object())
            row_configs.insert(row_configs.begin(), config["row"]);

        else if (config["row"].is_array())
            for (const auto &row_config : config["row"])
                row_configs.insert(row_configs.begin(), row_config);
    }

    if (config.count("col") != 0)
    {
        if (config["col"].is_object())
            col_configs.insert(col_configs.begin(), config["col"]);

        else if (config["col"].is_array())
            for (const auto &col_config : config["col"])
                col_configs.insert(col_configs.begin(), col_config);
    }

    if (config.count("position") != 0)
    {
        if (config["position"].is_object())
            position_configs.insert(position_configs.begin(), config["position"]);

        else if (config["position"].is_array())
            for (const auto &position_config : config["position"])
                position_configs.insert(position_configs.begin(), position_config);
    }
}

/**
 * {
 *     "row": {
 *         "where": {
 *             "mod": 2
 *             "text": "PAYDAY"
 *         }
 *         "sgr": 32
 *     }
 * }
 */
std::pair<std::vector<size_t>, Align> Table::Style::row(
    size_t row_n,
    const std::shared_ptr<std::vector<std::shared_ptr<Table::Cell>>> &cols) const
{
    std::vector<size_t> sgr;
    Align align(automatic);

    auto is_a_match = [&row_n, &cols](json row_config)
    {
        size_t expected = 0, matched = 0;

        if (row_config.count("where") == 0)
            return false;

        auto where = row_config["where"];

        if (expected == matched
            && where.count("n") != 0)
        {
            expected++;

            size_t n = where["n"].get<size_t>();

            if (row_n == n) matched++;
        }

        if (expected == matched
            && where.count("mod") != 0)
        {
            expected++;

            size_t mod = where["mod"].get<size_t>();

            if (row_n % mod != 0) matched++;
        }

        if (expected == matched
            && where.count("text") != 0)
        {
            expected++;

            std::string text = where["text"].get<std::string>();

            auto found = std::find_if(
                cols.get()->begin(),
                cols.get()->end(),
                [&text](auto &col)
                {
                    return col.get()->cmp(text);
                }
            );

            if (found != cols.get()->end()) matched++;
        }

        return expected > 0 && expected == matched;
    };

    auto get_alignment = [](json row_config)
    {
        auto alignment = row_config["align"].get<std::string>();

        return alignment == "left"   ? left
             : alignment == "center" ? center
             : alignment == "right"  ? right
             :                         automatic;
    };

    if (row_configs.size() > 0)
    {
        json row_sgr;

        for (const auto &row_config : row_configs)
        {
            if (is_a_match(row_config))
            {
                if (row_config.count("sgr") != 0)
                    row_sgr = row_config["sgr"];

                if (row_config.count("align") != 0)
                    align = get_alignment(row_config);
            }
        }

        if (row_sgr > 0)
        {
            if (row_sgr.is_array())
                for (const auto &number : row_sgr)
                    sgr.push_back(number.get<size_t>());
            else
                sgr.push_back(row_sgr.get<size_t>());
        }
    }

    return std::make_pair(sgr, align);
}

/**
 * {
 *     "col": {
 *         "where": {
 *             "n": 2
 *         }
 *         "sgr": 32
 *     }
 * }
 */
std::pair<std::vector<size_t>, Align> Table::Style::col(
    size_t col_n,
    const std::shared_ptr<Table::Cell> &col) const
{
    std::vector<size_t> sgr;
    Align align(automatic);

    auto is_a_match = [&col_n, &col](json col_config)
    {
        size_t expected = 0, matched = 0;

        if (col_config.count("where") == 0)
            return false;

        auto where = col_config["where"];

        if (expected == matched
            && where.count("n") != 0)
        {
            expected++;

            size_t n = where["n"].get<size_t>();

            if (col_n == n) matched++;
        }

        if (expected == matched
            && where.count("mod") != 0)
        {
            expected++;

            size_t mod = where["mod"].get<size_t>();

            if (col_n % mod != 0) matched++;
        }

        if (expected == matched
            && where.count("text") != 0)
        {
            expected++;

            std::string text = where["text"].get<std::string>();

            if (col.get()->cmp(text)) matched++;
        }

        return expected > 0 && expected == matched;
    };

    auto get_alignment = [](json col_config)
    {
        auto alignment = col_config["align"].get<std::string>();

        return alignment == "left"   ? left
             : alignment == "center" ? center
             : alignment == "right"  ? right
             :                         automatic;
    };

    if (col_configs.size() > 0)
    {
        json col_sgr;

        for (const auto &col_config : col_configs)
        {
            if (is_a_match(col_config))
            {
                if (col_config.count("sgr") != 0)
                    col_sgr = col_config["sgr"];

                if (col_config.count("align") != 0)
                    align = get_alignment(col_config);
            }
        }

        if (col_sgr > 0)
        {
            if (col_sgr.is_array())
                for (const auto &number : col_sgr)
                    sgr.push_back(number.get<size_t>());
            else
                sgr.push_back(col_sgr.get<size_t>());
        }
    }

    return std::make_pair(sgr, align);
}

/**
 * {
 *     "position": {
 *         "where": {
 *             "col_n": 2,
 *             "row_n": 2
 *         }
 *         "sgr": 32
 *     }
 * }
 */
std::pair<std::vector<size_t>, Align> Table::Style::position(
    size_t row_n,
    size_t col_n,
    const std::shared_ptr<Table::Cell> &col) const
{
    std::vector<size_t> sgr;
    Align align(automatic);

    auto is_a_match = [&row_n, &col_n, &col](json col_config)
    {
        size_t expected = 0, matched = 0;

        if (col_config.count("where") == 0)
            return false;

        auto where = col_config["where"];

        if (expected == matched
            && where.count("row_n") != 0
            && where.count("col_n") != 0)
        {
            expected++;

            size_t r_n = where["row_n"].get<size_t>();
            size_t c_n = where["col_n"].get<size_t>();

            if (row_n == r_n && col_n == c_n) matched++;
        }

        if (expected == matched
            && where.count("text") != 0)
        {
            expected++;

            std::string text = where["text"].get<std::string>();

            if (col.get()->cmp(text)) matched++;
        }

        return expected > 0 && expected == matched;
    };

    auto get_alignment = [](json position_config)
    {
        auto alignment = position_config["align"].get<std::string>();

        return alignment == "left"   ? left
             : alignment == "center" ? center
             : alignment == "right"  ? right
             :                         automatic;
    };

    if (position_configs.size() > 0)
    {
        json position_sgr;

        for (const auto &position_config : position_configs)
        {
            if (is_a_match(position_config))
            {
                if (position_config.count("sgr") != 0)
                    position_sgr = position_config["sgr"];

                if (position_config.count("align") != 0)
                    align = get_alignment(position_config);
            }
        }

        if (position_sgr > 0)
        {
            if (position_sgr.is_array())
                for (const auto &number : position_sgr)
                    sgr.push_back(number.get<size_t>());
            else
                sgr.push_back(position_sgr.get<size_t>());
        }
    }

    return std::make_pair(sgr, align);
}
