#include <iostream>
#include <memory>
#include <Reader.hpp>
#include "XmasParser.hpp"


int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Expected filename of input data!\n";
        return -1;
    }

    std::string inputName = argv[1];

    std::string filename = "../config/" + inputName;

    XmasParser parser(filename);

    std::cout << "Result: " << parser.countWords() << std::endl;

    return 0;
}