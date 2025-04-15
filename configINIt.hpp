#pragma once
#include <fstream>
#include <string>
#include <unordered_map>

class configINIt {
private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data;
    inline void TrimString(std::string& s);
    inline bool IsCommentLine(std::string& line);
    inline bool IsSectionLine(std::string& line);
    inline bool IsKeyValueLine(const std::string& line, size_t& pos);
public: 
    bool Load(const std::string& filename);
    std::string GetString(const std::string& section, const std::string& key, const std::string& default) const;
};

bool configINIt::Load(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        return false;
    }

    std::string line, section;

    while (std::getline(file, line))
    {
        TrimString(line);
        
        if (line.empty() || IsCommentLine(line))
        {
            continue;
        }

        if (IsSectionLine(line))
        {
            section = line.substr(1, line.length() - 2);
            TrimString(section);
        }
        else
        {
            size_t pos;
            std::string key, value;

            if (!IsKeyValueLine(line, pos))
            {
                continue;
            }

            key = line.substr(0, pos);
            value = line.substr(pos + 1);
            TrimString(key);
            TrimString(value);
            data[section][key] = value;
        }
    }

    return true;
}

std::string configINIt::GetString(const std::string& sectionKey, const std::string& key, const std::string& defaultValue) const
{
    auto section = data.find(sectionKey);
    if (section != data.end())
    {
        auto kvp = section->second.find(key);
        if (kvp != section->second.end())
        {
            return kvp->second;
        }
    }

    return defaultValue;
}

inline void configINIt::TrimString(std::string& s)
{
    const char* whitespace= " \t\r\n";
    size_t start = s.find_first_not_of(whitespace);
    if (start == std::string::npos)
    {
        return;
    }
    size_t end = s.find_last_not_of(whitespace);
    s = s.substr(start, end - start + 1);
}

inline bool configINIt::IsCommentLine(std::string& line)
{
    return (line[0] == ';') || (line[0] == '#');
}

inline bool configINIt::IsSectionLine(std::string& line)
{
    return (line[0] == '[') && (line.back() == ']');
}

inline bool configINIt::IsKeyValueLine(const std::string& line, size_t& pos)
{
    pos = line.find('=');
    return pos != std::string::npos;
}