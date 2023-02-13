#include <cmath>
#include <numeric>
#include <algorithm>
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

    std::string line;

    while (std::getline(ifs, line))
    {
        Table::Tokenizer tokens{line};

        auto cols = std::shared_ptr<std::vector<std::shared_ptr<Table::Cell>>>(new std::vector<std::shared_ptr<Table::Cell>>());

        if (tokens.begin() == tokens.end())
        {
            cols->push_back(std::shared_ptr<Table::Cell>(new Table::Cell("")));
        }
        else
        {
            for (const auto &token : tokens)
            {
                cols->push_back(std::shared_ptr<Table::Cell>(new Table::Cell(token)));
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
    header->push_back(std::shared_ptr<Table::Cell>(new Table::Cell(title)));
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

    std::for_each(columns->begin(), columns->end(),
        [&cols](std::string &col)
        {
            cols->push_back(std::shared_ptr<Table::Cell>(new Table::Cell(col)));
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

    rows.back()->push_back(std::shared_ptr<Table::Cell>(new Table::Cell(column)));
}

void Table::horizontal(std::ostream &out, std::vector<std::shared_ptr<std::vector<size_t>>> &width_for, size_t index) const
{
    auto left  = index == 0 ? TLEFT  : BLEFT;
    auto right = index == 0 ? TRIGHT : BRIGHT;
    auto tee   = index == 0 ? TTEE   : BTEE;

    index = index == 0 ? index : width_for.size() - 1;

    std::for_each(
        width_for.at(index)->begin(),
        width_for.at(index)->end(),
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

    size_t upper_col_n = width_for.at(upper_index)->size();
    size_t below_col_n = width_for.at(below_index)->size();

    if (upper_col_n == below_col_n)
        return;

    auto upper_vert = std::vector<bool>();
    upper_vert.push_back(true);

    std::for_each(
        width_for.at(upper_index)->begin(),
        width_for.at(upper_index)->end(),
        [&upper_vert](size_t width)
    {
        for (size_t i = 0; i < width; i++)
            upper_vert.push_back(false);

        upper_vert.push_back(true);
    });

    auto below_vert = std::vector<bool>();
    below_vert.push_back(true);

    std::for_each(
        width_for.at(below_index)->begin(),
        width_for.at(below_index)->end(),
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
        [&out, &row_n, &col_n, &width_for, this](decltype(*rows.begin()) &cols)
    {
        if (cols->begin() ==  cols->end())
            return;

        size_t col_count = width_for.at(row_n)->size();

        auto row_style = style.row(row_n, cols);

        col_n = 0;

        std::for_each(
            cols->begin(),
            cols->end(),
            [&out, &row_n, &col_n, &width_for, &col_count, &row_style, this](decltype(*cols->begin()) &col)
        {
            auto col_style = style.col(col_n, col);
            auto position_style = style.position(row_n, col_n, col);

            col->set_style(row_style, col_style, position_style);

            auto sgr = col->sgr_codes();
            Align align = col->alignment();

            size_t width = width_for.at(row_n)->at(col_n);
            size_t actual_width = col->length();
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
        [&row_n, &col_n, &max_col_count, &row_width, &max_row_width, &width_for](decltype(*rows.begin()) &cols)
    {
        col_n = 0;
        row_width = 0;

        auto widths = std::shared_ptr<std::vector<size_t>>(new std::vector<size_t>());

        if (cols->begin() ==  cols->end())
            return;

        std::for_each(
            cols->begin(),
            cols->end(),
            [&row_n, &col_n, &row_width, &widths, &width_for](decltype(*cols->begin()) &col)
        {
            size_t above_width = 0;
            size_t width = MIN_PADDING + col->length();

            for (int r_n = row_n - 1; r_n >= 0; r_n--)
            {
                if (width_for.at(r_n)->size() == 1
                    || width_for.at(r_n)->size() <= col_n)
                    continue;

                above_width = width_for.at(r_n)->at(col_n);
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
                    if (width_for.at(r_n)->size() == 1
                        || width_for.at(r_n)->size() <= col_n)
                        continue;

                    // update rows above with new wider width
                    width_for.at(r_n)->at(col_n) = width;
                }
            }

            widths->push_back(width);

            row_width += width;

            col_n++;
        });

        width_for.push_back(widths);

        size_t col_count = widths->size();
        size_t markup_count = 1 + col_count;

        max_col_count = std::max(col_count, max_col_count);
        max_row_width = std::max(row_width + markup_count, max_row_width);

        row_n++;
    });

    max_row_width = std::max(max_row_width, minimum_table_width);

    std::for_each(width_for.begin(), width_for.end(), [&max_row_width](decltype(*width_for.begin()) &cols)
    {
        size_t col_count = cols->size();
        size_t markup_count = 1 + col_count;

        size_t col_n = 0;

        while (std::accumulate(cols->begin(),
                               cols->end(),
                               markup_count) < max_row_width)
        {
            // bulk up columns until each row is the same width
            cols->at(col_n++) += 1;

            if (col_n == col_count) col_n = 0;
        }
    });

    return width_for;
}

std::ostream& operator << (std::ostream &out, const Table &t)
{
    return t.render(out);
}

Table::Tokenizer::Tokenizer(const std::string &line)
{
    const char esc = '\\';
    const char separator = ',';
    std::array<char, 2> last_two{' ', ' '};
    std::string token{};

    auto not_escaped = [](std::string &txt) -> bool
    {
        size_t count{0};
        if (txt.size() > 1)
            for (int i = txt.size() - 1; i > 0; i--)
            {
                if (txt[i] != esc) break;
                count++;
            }
        return count % 2 == 0;
    };

    for (const char &c : line)
    {
        if (c == separator && not_escaped(token))
        {
            tokens_.push_back(token);
            token.clear();
        }
        else
        {
            if (   token.length() > 1
                && token[token.length() - 1] == separator
                && token[token.length() - 2] == esc)
            {
                token[token.length() - 2] = token[token.length() - 1];
                token.pop_back();
            }
            token.push_back(c);
        }
        last_two[0] = last_two[1];
        last_two[1] = c;
    }

    if (token.length() > 0) tokens_.push_back(token);
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

bool Table::Cell::cmp(const std::string &subtext) const
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
    auto parse_style_conf = [](const json &config, const std::string &type) -> Table::Style::rule
    {
        const json where = config["where"];

        int row_n(where.count("row_n") ? where["row_n"].get<int>() : -1);
        int col_n(where.count("col_n") ? where["col_n"].get<int>() : -1);
        int mod(where.count("mod") ? where["mod"].get<int>() : -1);
        std::string text(where.count("text") ? where["text"].get<std::string>() : "");
        Align align(automatic);
        std::vector<size_t> sgr;

        if (config.count("align") > 0)
        {
            auto align_text = config["align"].get<std::string>();

            align = align_text == "left"   ? left
                  : align_text == "center" ? center
                  : align_text == "right"  ? right
                  :                          automatic;
        }

        if (where.count("n"))
        {
            if (type == "row")
            {
                row_n = where["n"].get<int>();
            }
            else if (type == "col")
            {
                col_n = where["n"].get<int>();
            }
            else if (type == "position")
            {
                row_n = where["n"].get<int>();
                col_n = where["n"].get<int>();
            }
        }

        if (config.count("sgr") > 0)
        {
            if (config["sgr"].is_array())
                for (const auto &number : config["sgr"])
                    sgr.push_back(number.get<size_t>());
            else
                sgr.push_back(config["sgr"].get<size_t>());
        }

        return Table::Style::rule(row_n, col_n, mod, text, align, sgr);
    };

    if (config.count("row") != 0)
    {
        if (config["row"].is_object()
            && config["row"].count("where") > 0)
        {
            row_rules.insert(
                row_rules.begin(),
                parse_style_conf(config["row"], "row")
            );
        }
        else if (config["row"].is_array())
        {
            for (const json &row : config["row"])
            {
                if (row.count("where") > 0)
                    row_rules.insert(
                        row_rules.begin(),
                        parse_style_conf(row, "row")
                    );
            }
        }
    }

    if (config.count("col") != 0)
    {
        if (config["col"].is_object()
            && config["col"].count("where") > 0)
        {
            col_rules.insert(
                col_rules.begin(),
                parse_style_conf(config["col"], "col")
            );
        }
        else if (config["col"].is_array())
        {
            for (const json &col : config["col"])
            {
                if (col.count("where") > 0)
                    col_rules.insert(
                        col_rules.begin(),
                        parse_style_conf(col, "col")
                    );
            }
        }
    }

    if (config.count("position") != 0)
    {
        if (config["position"].is_object()
            && config["position"].count("where") > 0)
        {
            pos_rules.insert(
                pos_rules.begin(),
                parse_style_conf(config["position"], "position")
            );
        }
        else if (config["position"].is_array())
        {
            for (const json &position : config["position"])
            {
                if (position.count("where") > 0)
                    pos_rules.insert(
                        pos_rules.begin(),
                        parse_style_conf(position, "position")
                    );
            }
        }
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

    if (row_rules.size() == 0)
        return std::make_pair(sgr, align);

    std::vector<size_t> row_sgr;

    std::for_each(
        row_rules.begin(),
        row_rules.end(),
        [&row_n, &cols, &row_sgr, &align](decltype(*row_rules.begin()) &rule)
        {
            size_t expected = 0, matched = 0;

            if (expected == matched
                && rule.row_n > -1)
            {
                expected++;

                if (row_n == (size_t)rule.row_n) matched++;
            }

            if (expected == matched
                && rule.mod > -1)
            {
                expected++;

                if (row_n % (size_t)rule.mod != 0) matched++;
            }

            if (expected == matched
                && rule.text.size() != 0)
            {
                expected++;

                auto found = std::find_if(
                    cols->begin(),
                    cols->end(),
                    [&rule](decltype(*cols->begin()) &col)
                    {
                        return col->cmp(rule.text);
                    }
                );

                if (found != cols->end()) matched++;
            }

            if (expected > 0 && expected == matched)
            {
                if (rule.sgr.size() != 0)
                    row_sgr = rule.sgr;

                if (rule.align != automatic)
                    align = rule.align;
            }
        }
    );

    if (row_sgr.size() > 0)
        sgr = row_sgr;

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

    if (col_rules.size() == 0)
        return std::make_pair(sgr, align);

    std::vector<size_t> col_sgr;

    std::for_each(
        col_rules.begin(),
        col_rules.end(),
        [&col_n, &col, &col_sgr, &align](decltype(*col_rules.begin()) &rule)
        {
            size_t expected = 0, matched = 0;

            if (expected == matched
                && rule.col_n > -1)
            {
                expected++;

                if (col_n == (size_t)rule.col_n) matched++;
            }

            if (expected == matched
                && rule.mod > -1)
            {
                expected++;

                if (col_n % (size_t)rule.mod != 0) matched++;
            }

            if (expected == matched
                && rule.text.size() != 0)
            {
                expected++;

                if (col->cmp(rule.text)) matched++;
            }

            if (expected > 0 && expected == matched)
            {
                if (rule.sgr.size() != 0)
                    col_sgr = rule.sgr;

                if (rule.align != automatic)
                    align = rule.align;
            }
        }
    );

    if (col_sgr.size() > 0)
        sgr = col_sgr;

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

    if (pos_rules.size() == 0)
        return std::make_pair(sgr, align);

    std::vector<size_t> position_sgr;

    std::for_each(
        pos_rules.begin(),
        pos_rules.end(),
        [&row_n, &col_n, &col, &position_sgr, &align](decltype(*pos_rules.begin()) &rule)
        {
            size_t expected = 0, matched = 0;

            if (expected == matched
                && rule.row_n > -1
                && rule.col_n > -1)
            {
                expected++;

                if (row_n == (size_t)rule.row_n && col_n == (size_t)rule.col_n) matched++;
            }

            if (expected == matched
                && rule.text.size() != 0)
            {
                expected++;

                if (col->cmp(rule.text)) matched++;
            }

            if (expected > 0 && expected == matched)
            {
                if (rule.sgr.size() != 0)
                    position_sgr = rule.sgr;

                if (rule.align != automatic)
                    align = rule.align;
            }
        }
    );

    if (position_sgr.size() > 0)
        sgr = position_sgr;

    return std::make_pair(sgr, align);
}
