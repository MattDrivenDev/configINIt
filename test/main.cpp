#include "../configINIt.hpp"
#include <filesystem>
#include <iostream>

int main()
{
    configINIt config;

    std::cout << "Loading config... ";    
    if (!config.Load("config.ini"))
    {
        std::cout << "Failed!\r\n";
        throw 1;
    }

    std::cout << "Done!\r\n";
    
    std::cout << "section1.key1: " << config.GetString("section1", "key1", "section1.key1 failed to read") << "\r\n";
    std::cout << "section1.key2: " << config.GetString("section1", "key2", "section1.key2 failed to read") << "\r\n";
    std::cout << "section2.key1: " << config.GetString("section2", "key1", "section2.key1 failed to read") << "\r\n";
    std::cout << "section2.key2: " << config.GetString("section2", "key2", "section2.key2 failed to read") << "\r\n";
}