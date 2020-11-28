#ifndef __CELL_HPP_INCLUDED__
#define __CELL_HPP_INCLUDED__


class Cell
{
public:
    Cell() = delete;
    Cell(std::string &text)
        : text(text),
        : size(text.size())
        {}

    size_t length();

private:
    std::string text;
    size_t size;
}

#endif
