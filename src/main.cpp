#include <fstream>
#include "table.hpp"


int main(int ac, const char *av[])
{
    if (ac < 2)
    {
        Table data = Table(std::cin);

        std::cout << data;
    }
    else
    {
        std::string filename(av[1]);

        auto data_ifs = std::ifstream(filename, std::ios::binary);

        if (data_ifs.good())
        {
            Table data = Table(data_ifs);

            std::cout << data;
        }
        else
        {
            std::cerr << "No such file: " << filename << std::endl;
            return 1;
        }
    }

    return 0;
}
