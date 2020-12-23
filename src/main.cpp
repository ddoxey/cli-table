#include <vector>
#include <boost/filesystem.hpp>
#include "table.hpp"

namespace fs = boost::filesystem;


void build_table(std::istream &data_ifs)
{
    Table data = Table(data_ifs);

    std::cout << data;
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
