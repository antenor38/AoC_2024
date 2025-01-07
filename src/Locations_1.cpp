#include "Locations_1.hpp"

Locations::Locations(std::string filename)
  : IReaderConsumer(filename)
{

}

void Locations::parseData()
{
    for(auto& line : mFileLines)
    {
        auto locations = mReader->splitString(line, " ");
        auto leftLoc = mReader->stringToNumber(locations.at(0));
        auto rightLoc = mReader->stringToNumber(locations.at(1));
        mLeftList.push_back(leftLoc);
        mRightList[rightLoc]++;
    }

    // std::sort(mLeftList.begin(), mLeftList.end());
    // std::sort(mRightList.begin(), mRightList.end());
}

int Locations::calculateDistance()
{
    int distance = 0;
    for(auto elem : mLeftList)
    {
        // distance += abs(mLeftList.at(i) - mRightList.at(i));
        distance += mRightList[elem] * elem;
    }
    return distance;
}