#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
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


Table::Table() {}

Table::Table(size_t width)
    : minimum_table_width(width)
    {}

Table::Table(std::string title, size_t width)
    : minimum_table_width(width)
{
    rows = std::vector<std::shared_ptr<std::vector<std::string>>>();
    add_header(title);
}

Table::Table(std::string title)
{
    rows = std::vector<std::shared_ptr<std::vector<std::string>>>();
    add_header(title);
}

void Table::add_header(const std::string &title)
{
    auto cols = std::shared_ptr<std::vector<std::string>>(new std::vector<std::string>());
    cols.get()->push_back(title);
    rows.push_back(cols);
}

void Table::add_row(const std::shared_ptr<std::vector<std::string>> &cols)
{
    rows.push_back(cols);
}

void Table::render() const
{
    size_t row_n = 0, col_n = 0;

    auto width_for = compute_widths_();

    horizontal(width_for, 0);

    std::for_each(
        rows.begin(),
        rows.end(),
        [&row_n, &col_n, &width_for, this](auto &cols)
    {
        col_n = 0;

        std::for_each(
            cols.get()->begin(),
            cols.get()->end(),
            [&row_n, &col_n, &width_for](std::string &col)
        {
            size_t col_count = width_for.at(row_n).get()->size();
            size_t width = width_for.at(row_n).get()->at(col_n);

            std::string pad_left(" ");
            std::string pad_right(" ");

            if (col_count == 1)
            {
                size_t left_size = std::ceil((width - col.length()) / 2);
                size_t right_size = left_size + (col.length() % 2 ? 0 : 1);
                pad_left.resize(left_size);
                std::fill(pad_left.begin(), pad_left.end(), ' ');
                pad_right.resize(right_size);
                std::fill(pad_right.begin(), pad_right.end(), ' ');
            }
            else
            {
                size_t right_size = width - ( 1 + col.length() );
                pad_right.resize(right_size);
                std::fill(pad_right.begin(), pad_right.end(), ' ');
            }

            std::cout << VERT << pad_left << col << pad_right;

            col_n++;
        });

        std::cout << VERT << std::endl;

        this->intermediate(width_for, row_n);

        row_n++;
    });

    horizontal(width_for, -1);
}

void Table::horizontal(std::vector<std::shared_ptr<std::vector<size_t>>> &width_for, size_t index) const
{
    auto left = index == 0 ? TLEFT : BLEFT;
    auto right = index == 0 ? TRIGHT : BRIGHT;
    auto tee = index == 0 ? TTEE : BTEE;

    index = index == 0 ? index : width_for.size() - 1;

    std::for_each(
        width_for.at(index).get()->begin(),
        width_for.at(index).get()->end(),
        [&left, &tee](size_t width)
    {
        std::cout << left;

        for (size_t i = 0; i < width; i++)
            std::cout << HORZ;

        left = tee;
    });

    std::cout << right
              << std::endl;
}

void Table::intermediate(std::vector<std::shared_ptr<std::vector<size_t>>> &width_for, size_t index) const
{
    if (index >= width_for.size() - 1)
        return;

    size_t col_n = width_for.at(index).get()->size();
    size_t below_col_n = width_for.at(index + 1).get()->size();

    auto tee = CROSS;

    if (below_col_n > col_n)
    {
        tee = TTEE;
        index += 1;
    }
    else if (below_col_n < col_n)
    {
        tee = BTEE;
    }
    else
    {
        return;
    }

    auto left = LTEE;
    auto right = RTEE;

    std::for_each(
        width_for.at(index).get()->begin(),
        width_for.at(index).get()->end(),
        [&left, &tee](size_t width)
    {
        std::cout << left;

        for (size_t i = 0; i < width; i++)
            std::cout << HORZ;

        left = tee;
    });

    std::cout << right
              << std::endl;
}

std::vector<std::shared_ptr<std::vector<size_t>>> Table::compute_widths_() const
{
    auto width_for = std::vector<std::shared_ptr<std::vector<size_t>>>();

    size_t row_n = 0,
           col_n = 0,
           max_col_n = 0,
           row_width = 0,
           max_row_width = 0;

    std::for_each(
        rows.begin(),
        rows.end(),
        [&row_n, &col_n, &max_col_n, &row_width, &max_row_width, &width_for](auto &cols)
    {
        max_col_n = std::max(col_n, max_col_n);
        max_row_width = std::max(row_width, max_row_width);

        col_n = 0;
        row_width = 0;

        auto widths = std::shared_ptr<std::vector<size_t>>(new std::vector<size_t>());

        std::for_each(
            cols.get()->begin(),
            cols.get()->end(),
            [&row_n, &col_n, &row_width, &widths, &width_for](std::string &col)
        {
            size_t above_width = 0;
            size_t width = MIN_PADDING + col.length();

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

        row_n++;
    });

    debug_widths_(width_for);

    max_row_width = std::max(
        max_row_width + (1 + max_col_n), // plus vertical bar chars
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

    debug_widths_(width_for);

    return width_for;
}

void Table::debug_widths_(std::vector<std::shared_ptr<std::vector<size_t>>> &width_for) const
{
    std::cout << "minimum_table_width: " << minimum_table_width << std::endl;

    std::for_each(
        width_for.begin(),
        width_for.end(),
        [](auto &cols)
    {
        size_t markup_chars = 1 + cols.get()->size();

        std::cout << markup_chars;

        std::for_each(
            cols.get()->begin(),
            cols.get()->end(),
            [](auto &width)
        {
            std::cout << " + " << width;
        });

        size_t total = std::accumulate(cols.get()->begin(), cols.get()->end(), markup_chars);

        std::cout << " = " << total << std::endl;
    });
}
