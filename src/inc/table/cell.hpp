#ifndef __CELL_HPP_INCLUDED__
#define __CELL_HPP_INCLUDED__

#include <string>
#include <iostream>


class Cell
{
public:
    Cell() = delete;
    Cell(const std::string &text);

    size_t length() const;
    std::ostream& str(std::ostream &out) const;

private:
    std::string text;
    size_t size;
};

std::ostream& operator << (std::ostream &out, const Cell &c);

#endif
