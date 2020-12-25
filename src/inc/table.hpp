#ifndef __TABLE_HPP_INCLUDED__
#define __TABLE_HPP_INCLUDED__

#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include "table/cell.hpp"
#include "boxchars.hpp"


class Table
{
public:

    Table();
    Table(const size_t width);
    Table(const std::string &title);
    Table(const std::string &title, const size_t width);
    Table(std::istream &ifs);

    void add_header(const std::string &title);
    void add_row(const std::shared_ptr<std::vector<std::string>> &cols);
    void add_col(const std::string &col);
    std::ostream& render(std::ostream &out) const;

    template<typename T>
    Table& operator << (T const &value)
    {
        std::stringstream text;
        text << value;
        add_col(text.str());
        return *this;
    }

private:
    size_t minimum_table_width = 0;
    std::vector<std::shared_ptr<std::vector<std::shared_ptr<Cell>>>> rows;

    void horizontal(std::ostream &out, std::vector<std::shared_ptr<std::vector<size_t>>> &width_for, size_t index) const;
    void intermediate(std::ostream &out, std::vector<std::shared_ptr<std::vector<size_t>>> &width_for, size_t index) const;

    std::vector<std::shared_ptr<std::vector<size_t>>> compute_widths_() const;
};

std::ostream& operator << (std::ostream &out, const Table &t);

#endif
