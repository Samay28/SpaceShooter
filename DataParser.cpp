#include "DataParser.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace
{   
    // Trim whitespace from both ends of a string
    std::string Trim(const std::string& input)
    {
        const auto first =
            input.find_first_not_of(" \t\r\n");

        if (first == std::string::npos)
        {
            return "";
        }

        const auto last =
            input.find_last_not_of(" \t\r\n");

        return input.substr(
            first,
            last - first + 1
        );
    }
}

std::vector<DataSection>
DataParser::Load(const char* filePath)
{
    std::vector<DataSection> sections;

    std::ifstream file(filePath);

    if (!file.is_open())
    {
        return sections;
    }

    DataSection* currentSection = nullptr;

    std::string line;

    while (std::getline(file, line))
    {
        line = Trim(line);

        if (line.empty())
        {
            continue;
        }

        if (line[0] == '#')
        {
            continue;
        }

        // ---------------- Section ----------------

        if (line.front() == '[' &&
            line.back() == ']')
        {
            DataSection section;

            section.name =
                line.substr(
                    1,
                    line.size() - 2
                );

            sections.push_back(
                std::move(section)
            );

            currentSection =
                &sections.back();

            continue;
        }

        // ---------------- Key = Value ----------------

        if (currentSection == nullptr)
        {
            continue;
        }

        const size_t equalsPosition =
            line.find('=');

        if (equalsPosition == std::string::npos)
        {
            continue;
        }

        std::string key =
            Trim(
                line.substr(
                    0,
                    equalsPosition
                )
            );

        std::string value =
            Trim(
                line.substr(
                    equalsPosition + 1
                )
            );

        currentSection->values[key] = value;
    }

    return sections;
}