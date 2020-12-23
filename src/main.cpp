#include <vector>
#include <boost/filesystem.hpp>
#include "table.hpp"

namespace fs = boost::filesystem;


int main(int ac, const char *av[])
{
    fs::path filename(ac > 1 ? av[1] : "");

    if (filename == "")
    {
        Table data = Table(std::cin);

        std::cout << data;
    }
    else if (fs::is_regular_file(filename))
    {
        auto data_ifs = std::ifstream(filename.string(), std::ios::binary);

        Table data = Table(data_ifs);

        std::cout << data;
    }
    else
    {
        std::cerr << "No such file: " << filename << std::endl;
        return 1;
    }

    return 0;
}
