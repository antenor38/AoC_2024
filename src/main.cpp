#include <iostream>
#include <memory>
#include <Reader.hpp>
#include "RedNoseReport_02.hpp"
#include "GardenPlots_12.hpp"
#include "MemoryRun_18.hpp"
#include "RobotPath_14.hpp"
#include "MonkeyMarket_22.hpp"


int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cerr << "Expected filename of input data!\n";
        return -1;
    }

    std::string inputName = argv[1];

    std::string filename = "../config/" + inputName;

    RedNoseReport reports(filename);
    reports.parseData();

    std::cout << "Result: " << reports.checkReports() << std::endl;

    return 0;
}