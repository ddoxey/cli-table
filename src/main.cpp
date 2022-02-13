#include <fstream>
#include "table.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;


int main(int ac, const char *av[])
{
    json style;
    std::string filename;

    for (int i = 1; i < ac; i++)
    {
        if (strcmp(av[i], "-s") == 0)
        {
            if (i + 1 == ac)
                throw std::invalid_argument("missing style parameter: -s <json|filename>");

            i++;
            std::string line;
            std::string style_text;
            std::string style_source(av[i]);

            std::ifstream style_ifs(style_source, std::ios::binary);

            if (style_ifs.good())
            {
                while (std::getline(style_ifs, line))
                    style_text.append(line);
            }
            else
            {
                style_text = style_source;
            }

            style = json::parse(style_text);
        }
        else
        {
            filename = av[i];
        }
    }

    if (filename.length() == 0)
    {
        Table data = Table(std::cin);

        if (style > 0)
            data.set_style(style);

        std::cout << data;
    }
    else
    {
        std::ifstream data_ifs(filename, std::ios::binary);

        if (data_ifs.good())
        {
            Table data = Table(data_ifs);

            if (style > 0)
                data.set_style(style);

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
