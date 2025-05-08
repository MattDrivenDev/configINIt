#include "../configINIt.hpp"
#include <filesystem>
#include <iostream>

int main()
{
    configINIt config;

    std::cout << "Loading config... ";    
    if (!config.Load("config.ini"))
    {
        std::cout << "Failed to load!\r\n";
        throw 1;
    }

    std::cout << "Done!\r\n";

    auto descriptionLines = config.GetRawLines("description");
    std::cout << "Description\r\n";
    for (std::string& line : descriptionLines)
    {
        std::cout << line << "\r\n";
    }
    std::cout << "\r\n";
    
    std::cout << "section1.key1: " << config.Get("section1", "key1", "section1.key1 failed to read") << "\r\n";
    std::cout << "section1.key2: " << config.Get("section1", "key2", "section1.key2 failed to read") << "\r\n";
    std::cout << "section2.key1: " << config.Get("section2", "key1", "section2.key1 failed to read") << "\r\n";
    std::cout << "section2.key2: " << config.Get("section2", "key2", "section2.key2 failed to read") << "\r\n";

    // What happens when we get something that doesn't exist? Oh - we have a default, but 
    // are we sure it doesn't also crash?
    std::cout << "section-does-not-exist: " << config.Get("nope", "key1", "correct!") << "\r\n";
    std::cout << "key-does-not-exist: " << config.Get("section1", "nope", "correct!") << "\r\n";

    // What about nulls?
    std::string nullstring;
    std::cout << "null-section-handling?: " << config.Get(nullstring, "key1", "correct!") << "\r\n";
    std::cout << "null-key-handling?: " << config.Get("section1", nullstring, "correct!") << "\r\n";

    config.Set("newsection", "key1", "hello world!");
    config.Set("section1", "key1", "you know when you've been tango'd");
    config.Set(" section2", "key2 ", " changed value ");

    if (!config.Save("config_new.ini"))
    {
        std::cout << "Failed to save!\r\n";
        throw 1;
    }
}