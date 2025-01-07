#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <algorithm>
#include <cmath>

#include "Reader.hpp"

class Locations : IReaderConsumer
{
public:
    Locations(std::string filename);

    void parseData() override;

    int calculateDistance();

private:
    std::vector<int> mLeftList;
    std::map<int, int> mRightList;
};