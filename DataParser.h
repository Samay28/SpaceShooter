#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct DataSection
{   
    // Each section has a name and a map of key-value pairs
    std::string name;
    std::unordered_map<std::string, std::string> values;
};

class DataParser
{
public:
    // Load a data file and return a vector of DataSection objects
    static std::vector<DataSection>
    Load(const char* filePath);
};