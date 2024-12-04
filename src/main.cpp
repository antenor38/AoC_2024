#include <iostream>
#include <memory>
#include <Reader.hpp>


int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Expected filename of input data!\n";
        return -1;
    }

    std::string inputName = argv[1];

    std::string filename = "../config/" + inputName;

    Reader reader(filename);

    auto lines = reader.readLines();


    std::cout << "Result: " << "0" << std::endl;

    return 0;
}