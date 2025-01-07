#include "LabGrid.hpp"

LabGrid::LabGrid(std::string filename)
  : mReader(std::make_unique<Reader>(filename))
  , mMaxX(0)
  , mMaxY(0)
  , mNumObstacles(0)
{
    mFileLines = mReader->readLines();
    populateGrid();
}

uint16_t LabGrid::calculateDistinctPositions()
{
    moveGuard(0); // initial direction is ^
    uint16_t size = static_cast<uint16_t>(mGuardPath.size());
    return size;
}

uint16_t* LabGrid::getDirectionPtr(uint8_t direction, int16_t& oper, coordinate* position)
{
    if(nullptr == position)
    {
        position = &mGuardPos;
    }

    uint16_t* axisPtr;
    oper = 1;

    if(0 == direction) // ^
    {
        oper *= -1;
        axisPtr = &(position->second);
        std::cout << "^";
    }
    else if(1 == direction) // >
    {
        axisPtr = &(position->first);
        std::cout << ">";
    }
    else if(2 == direction) // ˇ
    {
        axisPtr = &(position->second);
        std::cout << "ˇ";
    }
    else if(3 == direction) // <
    {
        oper *= -1;
        axisPtr = &(position->first);
        std::cout << "<";
    }

    return axisPtr;
}

void LabGrid::moveGuard(uint8_t direction, bool fakePath)
{
    std::cout << "Move ";
    auto const& x = mGuardPos.first;
    auto const& y = mGuardPos.second;
    uint16_t* axisPtr;
    uint16_t* nextAxisPtr;

    int16_t oper = 1, nextOper = 1;
    uint8_t nextDirection = (direction+1) % 4;

    axisPtr = getDirectionPtr(direction, oper, nullptr);

    (*axisPtr) += oper;
    while((x < mMaxX) && (y < mMaxY))
    {
        if(fakePath)
        {
            if(mFakeGuardPath.end() != mFakeGuardPath.find(mGuardPos))
            {
                auto const& directions = mFakeGuardPath.at(mGuardPos);
                if(directions.end() != directions.find(direction))
                {
                    mNumObstacles++;
                    return;
                }
            }
        }
        coordinate guardPosCopy = mGuardPos;
        nextAxisPtr = getDirectionPtr(nextDirection, nextOper, &guardPosCopy);
        std::cout << "\nx, y, oper = " << x << ", " << y << ", " << oper << std::endl;

        // reached an obstruction
        if(true == mLabGrid.at(coordinate(x,y)))
        {
            (*axisPtr) -= oper;
            moveGuard(nextDirection, fakePath);
            break;
        }
        else if(false == fakePath)
        {
            (*nextAxisPtr) += nextOper;
            if(true == checkObstructionCandidate(nextDirection, nextAxisPtr, nextOper, guardPosCopy))
            {
                guardPosCopy = mGuardPos;
                mFakeGuardPath = mGuardPath;
                mFakeGuardPath[mGuardPos].insert(nextDirection);
                moveGuard(nextDirection, true);
                mGuardPos = guardPosCopy;
            }
            else
            {
                std::cout << "X";
            }

        }
        std::cout << "\n";
        if(fakePath)
        {
            mFakeGuardPath[mGuardPos].insert(direction);
        }
        else
        {
            mGuardPath[mGuardPos].insert(direction);
        }
        (*axisPtr) += oper;
    }
}

bool LabGrid::checkObstructionCandidate(uint8_t direction, uint16_t* axisPtr, int16_t oper, coordinate position)
{
    auto const& x = position.first;
    auto const& y = position.second;

    // std::cout << "aux x = " << x << ", aux y = " << y << std::endl;
    while((x < mMaxX) && (y < mMaxY))
    {
        if(true == mLabGrid.at(coordinate(x,y)))
        {
            return true;
        }
        else if(mGuardPath.end() != mGuardPath.find(position))
        {
            auto const directions = mGuardPath[position];
            if(directions.end() != directions.find(direction))
            {
                return true;
            }
        }
        (*axisPtr) += oper;
    }
    return false;
}

void LabGrid::populateGrid()
{
    mMaxX = mFileLines.at(0).length();
    mMaxY = mFileLines.size();

    for(uint16_t y = 0; y < mMaxY; y++)
    {
        for(uint16_t x = 0; x < mMaxX; x++)
        {
            char const c = mFileLines.at(y).at(x);
            coordinate position(x, y);
            obstruction obs(false);
            if('#' == c)
            {
                obs = true;
            }
            else if ('^' == c)
            {
                mGuardPath[position] = {};
                mGuardPos = position;
            }
            mLabGrid[position] = obs;
        }
    }
}
