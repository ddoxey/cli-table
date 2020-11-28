#include <vector>
#include <istream>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include "table.hpp"

namespace fs = boost::filesystem;


void build_table(std::istream &data_ifs)
{
    using tokenizer = boost::tokenizer<boost::escaped_list_separator<char>>;

    Table data = Table();

    std::string line;

    while (std::getline(data_ifs, line))
    {
        tokenizer tokens{line};

        auto cols = std::shared_ptr<std::vector<std::string>>(new std::vector<std::string>());
        for (const auto &token : tokens)
        {
            cols.get()->push_back(token);
        }

        data.add_row(cols);
    }

    data.render();
}


int main(int ac, const char *av[])
{
    fs::path filename(ac > 1 ? av[1] : "");

    if (filename == "")
    {
        build_table(std::cin);
    }
    else if (fs::is_regular_file(filename))
    {
        auto data_ifs = std::ifstream(filename.string(), std::ios::binary);

        build_table(data_ifs);
    }
    else
    {
        std::cerr << "No such file: " << filename << std::endl;
        return 1;
    }

    return 0;
}
