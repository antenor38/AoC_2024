#include <iostream>
#include <memory>
#include <Reader.hpp>
#include "GardenPlots_12.hpp"


int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cerr << "Expected filename of input data!\n";
        return -1;
    }

    std::string inputName = argv[1];

    std::string filename = "../config/" + inputName;

    GardenPlots plots(filename);

    plots.parseField();

    std::cout << "Result: " << plots.totalFencePrice() << std::endl;

    return 0;
}