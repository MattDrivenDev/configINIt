#pragma once
#include <fstream>
#include <string>
#include <unordered_map>

class configINIt {
private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> data;

    inline void TrimString(std::string& s)
    {
        if (s.empty())
        {
            return;
        }

        const char* whitespace= " \t\r\n";
        size_t start = s.find_first_not_of(whitespace);
        if (start == std::string::npos)
        {
            return;
        }
        size_t end = s.find_last_not_of(whitespace);
        s = s.substr(start, end - start + 1);
    }

    inline bool IsCommentLine(const std::string& line)
    {
        return !line.empty() && (line[0] == ';') || (line[0] == '#');
    }

    inline bool IsSectionLine(const std::string& line)
    {
        return !line.empty() && (line[0] == '[') && (line.back() == ']');
    }

    inline bool IsKeyValueLine(const std::string& line, size_t& pos)
    {
        if (line.empty())
        {
            return false;
        }

        pos = line.find('=');
        return pos != std::string::npos;
    }

public: 
    bool Load(const std::string& filename)
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

        file.close(); // optional - ifstream closes automatically when destructor is called
    
        return true;
    }

    bool Save(const std::string& filename)
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            return false;
        }

        for (const std::pair<const std::string, std::unordered_map<std::string, std::string>>& section : data)
        {
            file << "[" << section.first << "]" << std::endl;

            for (const std::pair<const std::string, std::string>& kvp : section.second)
            {
                file << kvp.first << "=" << kvp.second << std::endl;
            }

            file << std::endl;
        }

        file.close(); // optional - ofstream closes automatically when destructor is called

        return true;
    }

    std::string Get(const std::string& sectionKey, const std::string& key, const std::string& defaultValue) const
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

    void Set(const std::string& sectionKey, const std::string& key, const std::string& value)
    {
        data[sectionKey][key] = value;
    }
};