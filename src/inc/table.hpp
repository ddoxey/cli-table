#ifndef __TABLE_HPP_INCLUDED__
#define __TABLE_HPP_INCLUDED__

#include <string>
#include <memory>
#include <vector>
#include "boxchars.hpp"


class Table
{
public:

    Table();
    Table(size_t width);
    Table(std::string title);
    Table(std::string title, size_t width);

    void add_header(const std::string &title);
    void add_row(const std::shared_ptr<std::vector<std::string>> &cols);
    void render() const;

private:
    size_t minimum_table_width = 0;
    std::vector<std::shared_ptr<std::vector<std::string>>> rows;

    void horizontal(std::vector<std::shared_ptr<std::vector<size_t>>> &width_for, size_t index) const;
    void intermediate(std::vector<std::shared_ptr<std::vector<size_t>>> &width_for, size_t index) const;
    
    std::vector<std::shared_ptr<std::vector<size_t>>> compute_widths_() const;
    void debug_widths_(std::vector<std::shared_ptr<std::vector<size_t>>> &width_for) const;
};

#endif
