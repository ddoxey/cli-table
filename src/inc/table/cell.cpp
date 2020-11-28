#include <string>
#include "cell.hpp"


Cell::Cell(const std::string &text)
    : text(text),
      size(text.size())
    {}

size_t Cell::length() const
{
    return size;
}

std::ostream& Cell::str(std::ostream &out) const
{
    return out << text;
}

std::ostream& operator << (std::ostream &out, const Cell &c)
{
    return c.str(out);
}
