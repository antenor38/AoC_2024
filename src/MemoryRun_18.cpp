#include "MemoryRun_18.hpp"

MemoryRun::MemoryRun(std::string filename)
  : IReaderConsumer(filename)
  , mPosition(0,0)
  , mTargetPos(kWidthX, kHeightY)
  , mCriticalByte(0,0)
{
    for(size_t i = 0; i <= kWidthX; i++)
    {
        for(size_t j = 0; j <= kHeightY; j++)
        {
            std::shared_ptr<MemoryPosition> memPos = std::make_shared<MemoryPosition>();
            memPos->pos = coordinate(i,j);
            memPos->distance = (kWidthX - i) + (kHeightY - j);
            memPos->partOfPath = false;
            memPos->calculated = false;
            memPos->shortest = false;
            if(i > 0)
            {
                memPos->neighbours.push_back(mMemoryMap[coordinate(i-1, j)]);
                mMemoryMap[coordinate(i-1,j)]->neighbours.push_back(memPos);
            }
            if(j > 0)
            {
                memPos->neighbours.push_back(mMemoryMap[coordinate(i, j-1)]);
                mMemoryMap[coordinate(i,j-1)]->neighbours.push_back(memPos);
            }
            mMemoryMap[coordinate(i, j)] = memPos;
        }
    }
}

void MemoryRun::parseData()
{
    for(auto& line : mFileLines)
    {
        auto coordinates = mReader->splitString(line, ",");
        auto xCoordinate = mReader->stringToNumber(coordinates.at(0));
        auto yCoordinate = mReader->stringToNumber(coordinates.at(1));
        mFallingBytes.push_back(coordinate(xCoordinate, yCoordinate));
    }
    std::cout << "Parsed data with " << mFallingBytes.size() << " falling bytes\n";
}

uint32_t MemoryRun::traverseMemoryPath(int steps)
{
    uint32_t shortestPath = 0;

    size_t i = 0;
    while(steps > 0 && i < mFallingBytes.size())
    {
        mMemoryMap[mFallingBytes.at(i)]->distance = UINT32_MAX;
        i++;
        steps--;
    }

    std::cout << "Marked corrupt bytes\n";

    printMap();

    coordinate fallingByte(0,0);

    // while(UINT16_MAX > shortestPath && i < 3450)
    // {
        shortestPath = calculateShortestDistance(mMemoryMap[mPosition], mMemoryMap[coordinate(kWidthX, kHeightY)]);
    //     resetCalculated();
    //     fallingByte = mFallingBytes.at(i++);
    //     mMemoryMap[fallingByte]->distance = UINT32_MAX;
    // }

    mCriticalByte = fallingByte;

    markShortest(mMemoryMap[mPosition]);

    for(auto j = 0; j <= kHeightY; j++)
    {
        for(auto i = 0; i <= kWidthX; i++)
        {
            if(UINT32_MAX == mMemoryMap[coordinate(i,j)]->distance)
            {
                std::cout << "#";
            }
            else if (true == mMemoryMap[coordinate(i,j)]->shortest)
            {
                std::cout << "O";
            }
            else
            {
                std::cout << ".";
            }
        }
        std::cout << "\n";
    }

    return shortestPath;
}

void MemoryRun::markShortest(std::shared_ptr<MemoryPosition> pos)
{
    std::shared_ptr<MemoryPosition> best;

    uint32_t minDist = pos->distance;
    for(auto elem : pos->neighbours)
    {
        if(elem->distance < minDist)
        {
            minDist = elem->distance;
            best = elem;
        }
    }
    pos->shortest = true;
    if(minDist < pos->distance)
    {
        markShortest(best);
    }
}

void MemoryRun::printMap()
{
    for(auto j = 0; j <= kHeightY; j++)
    {
        for(auto i = 0; i <= kWidthX; i++)
        {
            if(UINT32_MAX == mMemoryMap[coordinate(i,j)]->distance)
            {
                std::cout << "#";
            }
            else
            {
                std::cout << ".";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n___________________________________________________________________________________\n";
}

void MemoryRun::resetCalculated()
{
    for(auto& elem : mMemoryMap)
    {
        if(UINT32_MAX == elem.second->distance && true == elem.second->calculated)
        {
            elem.second->distance = (kWidthX - elem.first.first) + (kHeightY - elem.first.second); 
        }
        elem.second->calculated = false;
    }
}

uint32_t MemoryRun::calculateShortestDistance(std::shared_ptr<MemoryPosition> start, std::shared_ptr<MemoryPosition> end)
{
    // std::cout << "->Pos(" << start->pos.first << "," << start->pos.second << ") ";
    if(start == end)
    {
        // std::cout << "Pos(" << start->pos.first << "," << start->pos.second << "): 0\n";
        return 0;
    }

    if(UINT32_MAX == start->distance)
    {
        // std::cout << "Pos(" << start->pos.first << "," << start->pos.second << "): UINT32_MAX\n";
        return UINT32_MAX;
    }

    if(true == start->calculated)
    {
        return start->distance;
    }

    start->partOfPath = true;
    bool pathInFront = false;
    uint32_t minDist = UINT32_MAX;

    for(auto it = start->neighbours.begin(); it != start->neighbours.end() ; )
    {
        auto neighbour = *it;
        if(false == neighbour->partOfPath)
        {
            auto value = calculateShortestDistance(neighbour, end);
            if(value < minDist)
            {
                minDist = value;
            }
            // if(UINT16_MAX == value)
            // {
            //     start->neighbours.erase(it);
            //     continue;
            // }
        }
        else
        {
            pathInFront = true;
        }
        it++;
    }

    start->partOfPath = false;
    if(UINT16_MAX > minDist)
    {
        start->distance = minDist;
        start->calculated = true;
        start->distance++;
    }
    else
    {
        if(false == pathInFront)
        {
            start->distance = UINT32_MAX;
            start->calculated = true;
        }
        else
        {
            return UINT16_MAX;
        }
        // if(mCounter++ > 80 && mCounter < 200)
        // {
        //     printMap();
        // }
    }

    // std::cout << "Pos(" << start->pos.first << "," << start->pos.second << "): " << start->distance << "(neighbours count: " << start->neighbours.size() << ")\n";
    return start->distance;
}   