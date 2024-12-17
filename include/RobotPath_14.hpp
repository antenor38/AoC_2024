#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "Reader.hpp"

typedef std::pair<uint16_t, uint16_t> coordinate;
typedef std::pair<int16_t, int16_t> velocity;

class Robot
{
public:
    Robot(uint16_t const x, uint16_t const y, int16_t const xSpeed, int16_t const ySpeed, uint16_t widthX, uint16_t heightY);

    coordinate calculatePosition(uint16_t const steps = 1);

private:
    uint16_t mWidthX;
    uint16_t mHeightY;
    coordinate mPosition;
    velocity mVelocity;
};

class RobotPath
{
public:
    RobotPath(std::string filename);

    void parseData();

    uint32_t calculateSafetyFactor();

private:
    size_t detectQuadrant(coordinate pos);

    std::vector<std::unique_ptr<Robot>> mRobots;
    std::vector<std::map<coordinate, uint16_t>> mRobotsOnFloor;

    std::vector<std::string> mFileLines;
    std::unique_ptr<Reader> mReader;

    uint16_t const kWidthX = 11;
    uint16_t const kHeightY = 7;
    bool mAvoidX, mAvoidY;
};