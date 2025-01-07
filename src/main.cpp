#include <iostream>
#include <memory>
#include <Reader.hpp>
#include "GardenPlots_12.hpp"
#include "MemoryRun_18.hpp"


int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cerr << "Expected filename of input data!\n";
        return -1;
    }

    std::string inputName = argv[1];

    std::string filename = "../config/" + inputName;

    MemoryRun memRun(filename);
    memRun.parseData();

    std::cout << "Result: " << memRun.traverseMemoryPath(1024) << std::endl;

    std::cout << "Critical byte : (" << memRun.getCriticalByte().first << "," << memRun.getCriticalByte().second << ")\n";

    return 0;
}