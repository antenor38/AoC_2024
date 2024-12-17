#include "RobotPath_14.hpp"

Robot::Robot(uint16_t const x, uint16_t const y, int16_t const xSpeed, int16_t const ySpeed, uint16_t widthX, uint16_t heightY)
  : mWidthX(widthX)
  , mHeightY(heightY)
  , mPosition(x,y)
  , mVelocity(xSpeed, ySpeed)
{
}

coordinate Robot::calculatePosition(uint16_t const steps)
{
    if(0 < mVelocity.first)
    {
        mPosition.first = (mPosition.first + (mVelocity.first * steps)) % mWidthX;
    }
    else if(0 > mVelocity.first)
    {
        uint16_t tempSteps = steps;
        int16_t move = (mVelocity.first * steps) % mWidthX;

        if(mPosition.first < (move * -1))
        {
            move += mPosition.first;
            mPosition.first = mWidthX;
        }
        mPosition.first += move;

    }
    if(0 < mVelocity.second)
    {
        mPosition.second = (mPosition.second + (mVelocity.second * steps)) % mHeightY;
    }
    else if(0 > mVelocity.second)
    {
        uint16_t tempSteps = steps;
        int16_t move = (mVelocity.second * steps) % mHeightY;

        if(mPosition.second < (move * -1))
        {
            move += mPosition.second;
            mPosition.second = mHeightY;
        }
        mPosition.second += move;
    }

    return mPosition;
}

RobotPath::RobotPath(std::string filename)
  : mReader(std::make_unique<Reader>(filename))
  , mAvoidX(false)
  , mAvoidY(false)
{
    mRobotsOnFloor.resize(4);
    mFileLines = mReader->readLines();
    if(kWidthX % 2 == 1)
    {
        mAvoidX = true;
    }
    if(kHeightY % 2 == 1)
    {
        mAvoidY = true;
    }
}

void RobotPath::parseData()
{
    for(auto stringLine : mFileLines)
    {
        auto line = mReader->splitString(stringLine, " ");
        auto posPart = mReader->splitString(line.at(0), "=");
        auto posCoordinates = mReader->splitString(posPart.at(1), ",");
        uint16_t x = static_cast<uint16_t>(mReader->stringToNumber(posCoordinates.at(0)));
        uint16_t y = static_cast<uint16_t>(mReader->stringToNumber(posCoordinates.at(1)));

        auto velPart = mReader->splitString(line.at(1), "=");
        auto velSpeeds = mReader->splitString(velPart.at(1), ",");
        uint16_t xSpeed = static_cast<uint16_t>(mReader->stringToNumber(velSpeeds.at(0)));
        uint16_t ySpeed = static_cast<uint16_t>(mReader->stringToNumber(velSpeeds.at(1)));

        mRobots.push_back(std::make_unique<Robot>(x, y, xSpeed, ySpeed, kWidthX, kHeightY));
    }
}

size_t RobotPath::detectQuadrant(coordinate pos)
{
    if(pos.first < (kWidthX / 2))
    {
        if(pos.second < (kHeightY / 2))
        {
            return 0;
        }
        else if((pos.second > (kHeightY / 2)) || (pos.second == (kHeightY / 2) && false == mAvoidY))
        {
            return 2;
        }
    }
    else if((pos.first > (kWidthX / 2)) || (pos.first == (kWidthX / 2) && false == mAvoidX))
    {
        if(pos.second < (kHeightY / 2))
        {
            return 1;
        }
        else if((pos.second > (kHeightY / 2)) || (pos.second == (kHeightY / 2) && false == mAvoidY))
        {
            return 3;
        }
    }

    return 5;
}

uint32_t RobotPath::calculateSafetyFactor()
{
    for(auto& robot : mRobots)
    {
        auto pos = robot->calculatePosition(100);
        size_t quadrant = detectQuadrant(pos);
        if(5 > quadrant)
        {
            mRobotsOnFloor.at(quadrant)[pos]++;
        }
    }

    uint32_t sumAll = 1;

    for(auto& quadrant : mRobotsOnFloor)
    {
        uint32_t sum = 0;

        for(auto& elem : quadrant)
        {
            sum += elem.second;
        }
        sumAll *= sum;
    }

    return sumAll;
}
