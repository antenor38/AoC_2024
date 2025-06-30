#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

class Reader
{
public:
    Reader(std::string filename);

    std::vector<std::string> readLines();

    static std::vector<std::string> splitString(std::string line, std::string delimiter);

    static uint64_t stringToNumber(std::string numStr);

    bool parseUInt(std::string const& candidate, uint64_t& resultNum, char const delimiter = '\0');

    void changeFile(std::string filename);
private:

    std::string mFilename;
};

class IReaderConsumer
{
public:
    IReaderConsumer(std::string filename)
  : mReader(std::make_unique<Reader>(filename))
{
    mFileLines = mReader->readLines();
}

    virtual void parseData()
    {
        std::cout << "Missing implementation of parseData()\n";
    }

protected:
    std::vector<std::string> mFileLines;
    std::unique_ptr<Reader> mReader;
};