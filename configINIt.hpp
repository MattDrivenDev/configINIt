#pragma once
#include <fstream>
#include <string>
#include <unordered_map>

struct configSection
{
    std::unordered_map<std::string, std::string> kvps;
    std::vector<std::string> rawLines;
};

/// @brief Useful for unit testing and mocking.
class configBase {
public:
    virtual ~configBase() = default;
    virtual std::string Get(const std::string& sectionKey, const std::string& key, const std::string& defaultValue) const = 0;
    virtual size_t GetKeyCount(const std::string& sectionKey) const = 0;
    virtual std::vector<std::string> GetKeys(const std::string& sectionKey) const = 0;
    virtual std::vector<std::string> GetRawLines(const std::string& sectionKey) const = 0;
    virtual void Set(std::string sectionKey, std::string key, std::string value) = 0;
    virtual void SetRawLines(std::string sectionKey, std::vector<std::string> values) = 0;
};

class configINIt : public configBase {
private:
    std::unordered_map<std::string, configSection> data;

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
    
                if (IsKeyValueLine(line, pos))
                {
                    key = line.substr(0, pos);
                    value = line.substr(pos + 1);
                    TrimString(key);
                    TrimString(value);
                    data[section].kvps[key] = value;
                }
                else
                {
                    data[section].rawLines.push_back(line);
                }                
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

        for (const std::pair<const std::string, configSection>& section : data)
        {
            file << "[" << section.first << "]" << std::endl;

            for (const std::string& rawLine : section.second.rawLines)
            {
                file << rawLine << std::endl;
            }

            for (const std::pair<const std::string, std::string>& kvp : section.second.kvps)
            {
                file << kvp.first << "=" << kvp.second << std::endl;
            }

            file << std::endl;
        }

        file.close(); // optional - ofstream closes automatically when destructor is called

        return true;
    }

    size_t GetKeyCount(const std::string& sectionKey) const override
    {
        auto section = data.find(sectionKey);
        if (section != data.end())
        {
            return section->second.kvps.size();
        }
    
        return 0;
    }

    std::vector<std::string> GetKeys(const std::string& sectionKey) const override
    {
        auto section = data.find(sectionKey);
        if (section != data.end())
        {
            std::vector<std::string> keys;
            for (const auto& kvp : section->second.kvps)
            {
                keys.push_back(kvp.first);
            }
            return keys;
        }
    
        return {};
    }

    std::string Get(const std::string& sectionKey, const std::string& key, const std::string& defaultValue) const override
    {
        auto section = data.find(sectionKey);
        if (section != data.end())
        {
            auto kvp = section->second.kvps.find(key);
            if (kvp != section->second.kvps.end())
            {
                return kvp->second;
            }
        }
    
        return defaultValue;
    }

    std::vector<std::string> GetRawLines(const std::string& sectionKey) const override
    {
        auto section = data.find(sectionKey);
        if (section != data.end())
        {
            auto rawLines = section->second.rawLines;
            return rawLines;
        }

        return {};
    }

    void Set(std::string sectionKey, std::string key, std::string value) override
    {
        // Let's not allow empty section or key names
        if (sectionKey.empty() || key.empty())
        {
            return;
        }

        // When we load, we trim the strings so that the config is clean. 
        // So, let's do the same when we set a value too.
        TrimString(sectionKey);
        TrimString(key);
        TrimString(value);

        data[sectionKey].kvps[key] = value;
    }

    void SetRawLines(std::string sectionKey, std::vector<std::string> values) override
    {
        // Ignore empty section or values
        if (sectionKey.empty() || values.empty())
        {
            return;
        }

        // We'll trim.
        TrimString(sectionKey);

        data[sectionKey].rawLines = values;
    }
};