#include <cmath>
#include <numeric>
#include <algorithm>
#include <boost/tokenizer.hpp>
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


Table::Table()
{
    rows = std::vector<std::shared_ptr<std::vector<std::shared_ptr<Cell>>>>();
}

Table::Table(const size_t width)
    : minimum_table_width(width)
{
    rows = std::vector<std::shared_ptr<std::vector<std::shared_ptr<Cell>>>>();
}

Table::Table(const std::string &title, const size_t width)
    : minimum_table_width(width)
{
    rows = std::vector<std::shared_ptr<std::vector<std::shared_ptr<Cell>>>>();
    add_header(title);
}

Table::Table(const std::string &title)
{
    rows = std::vector<std::shared_ptr<std::vector<std::shared_ptr<Cell>>>>();
    add_header(title);
}

Table::Table(std::istream &ifs)
{
    rows = std::vector<std::shared_ptr<std::vector<std::shared_ptr<Cell>>>>();

    using tokenizer = boost::tokenizer<boost::escaped_list_separator<char>>;

    std::string line;

    while (std::getline(ifs, line))
    {
        tokenizer tokens{line};

        auto cols = std::shared_ptr<std::vector<std::shared_ptr<Cell>>>(new std::vector<std::shared_ptr<Cell>>());

        if (tokens.begin() == tokens.end())
        {
            cols.get()->push_back(std::shared_ptr<Cell>(new Cell("")));
        }
        else
        {
            for (const auto &token : tokens)
            {
                cols.get()->push_back(std::shared_ptr<Cell>(new Cell(token)));
            }
        }

        rows.push_back(cols);
    }
}

void Table::operator << (const std::string &text)
{
    if (text == "\n" || rows.size() == 0)
    {
        auto cols = std::shared_ptr<std::vector<std::shared_ptr<Cell>>>(new std::vector<std::shared_ptr<Cell>>());

        rows.push_back(cols);

        if (text == "\n")
            return;
    }

    rows.back().get()->push_back(std::shared_ptr<Cell>(new Cell(text)));
}

void Table::add_header(const std::string &title)
{
    auto cols = std::shared_ptr<std::vector<std::shared_ptr<Cell>>>(new std::vector<std::shared_ptr<Cell>>());
    cols.get()->push_back(std::shared_ptr<Cell>(new Cell(title)));
    rows.push_back(cols);
}

void Table::add_row(const std::shared_ptr<std::vector<std::string>> &columns)
{
    auto cols = std::shared_ptr<std::vector<std::shared_ptr<Cell>>>(new std::vector<std::shared_ptr<Cell>>());

    std::for_each(columns.get()->begin(), columns.get()->end(),
        [&cols](std::string &col)
        {
            cols.get()->push_back(std::shared_ptr<Cell>(new Cell(col)));
        }
    );

    rows.push_back(cols);
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

        max_col_count = std::max(widths.get()->size(), max_col_count);

        width_for.push_back(widths);

        max_row_width = std::max(row_width, max_row_width);

        row_n++;
    });

    max_row_width = std::max(
        max_row_width + (1 + max_col_count), // plus vertical bar chars
        minimum_table_width
    );

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
        col_n = 0;

        std::for_each(
            cols.get()->begin(),
            cols.get()->end(),
            [&out, &row_n, &col_n, &width_for](auto &col)
        {
            size_t col_count = width_for.at(row_n).get()->size();
            size_t width = width_for.at(row_n).get()->at(col_n);

            std::string pad_left(" ");
            std::string pad_right(" ");

            if (col_count == 1)
            {
                size_t left_size = std::ceil((width - col.get()->length()) / 2);
                size_t right_size = left_size + (col.get()->length() % 2 ? 0 : 1);
                pad_left.resize(left_size);
                std::fill(pad_left.begin(), pad_left.end(), ' ');
                pad_right.resize(right_size);
                std::fill(pad_right.begin(), pad_right.end(), ' ');
            }
            else
            {
                size_t right_size = width - ( 1 + col.get()->length() );
                pad_right.resize(right_size);
                std::fill(pad_right.begin(), pad_right.end(), ' ');
            }

            out << VERT << pad_left << *col.get() << pad_right;

            col_n++;
        });

        out << VERT << std::endl;

        this->intermediate(out, width_for, row_n);

        row_n++;
    });

    horizontal(out, width_for, -1);

    return out;
}

std::ostream& operator << (std::ostream &out, const Table &t)
{
    return t.render(out);
}
