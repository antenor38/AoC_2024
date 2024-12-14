#include <iostream>
#include <memory>
#include <Reader.hpp>
#include "RobotPath_14.hpp"


int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "Expected filename of input data!\n";
        return -1;
    }

    std::string inputName = argv[1];

    std::string filename = "../config/" + inputName;

    RobotPath robotPath(filename);
    robotPath.parseData();

    std::cout << "Result: " << robotPath.calculateSafetyFactor() << std::endl;

    return 0;
}