#include "../configINIt.hpp"
#include <iostream>

int main()
{
    std::string section1Key1, section1Key2, section2Key1, section2Key2;
    configINIt config;

    std::cout << "Loading config... ";

    config.Load("config.ini");

    std::cout << "Done!\r\n";

    section1Key1 = config.GetString("section1", "key1", "section1.key1 failed to read");
    section1Key2 = config.GetString("section1", "key2", "section1.key2 failed to read");
    section2Key1 = config.GetString("section2", "key1", "section2.key1 failed to read");
    section2Key2 = config.GetString("section2", "key2", "section2.key2 failed to read");

    std::cout << "section1.key1: " << section1Key1 << "\r\n";
    std::cout << "section1.key2: " << section1Key2 << "\r\n";
    std::cout << "section2.key1: " << section2Key1 << "\r\n";
    std::cout << "section2.key2: " << section2Key2 << "\r\n";
}