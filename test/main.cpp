#include "../configINIt.hpp"
#include <iostream>

int main()
{
    std::string section1Key1, section1Key2;
    configINIt config;

    std::cout << "Loading config... ";

    config.Load("config.ini");

    std::cout << "Done!\r\n";

    section1Key1 = config.GetString("section1", "key1", "section one key1 failed to read");
    section1Key2 = config.GetString("section1", "key2", "section one key2 failed to read");

    std::cout << "section1.key1: " << section1Key1 << "\r\n";
    std::cout << "section1.key2: " << section1Key2 << "\r\n";
}