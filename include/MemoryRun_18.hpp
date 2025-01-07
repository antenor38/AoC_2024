#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>

#include "Reader.hpp"

typedef std::pair<uint16_t, uint16_t> coordinate;

struct MemoryPosition
{
    coordinate pos;
    uint32_t distance;
    bool partOfPath;
    bool calculated;
    bool shortest;
    std::vector<std::shared_ptr<MemoryPosition>> neighbours;
};

class MemoryRun : public IReaderConsumer
{
public:
    MemoryRun(std::string filename);

    void parseData() override;

    uint32_t traverseMemoryPath(int steps = 1);

    void markShortest(std::shared_ptr<MemoryPosition> pos);

    coordinate getCriticalByte() const
    {
        return mCriticalByte;
    }

private:
    uint32_t calculateShortestDistance(std::shared_ptr<MemoryPosition> start, std::shared_ptr<MemoryPosition> end);

    void printMap();

    void resetCalculated();

    uint16_t const kWidthX = 70;
    uint16_t const kHeightY = 70;
    std::map<coordinate, std::shared_ptr<MemoryPosition>> mMemoryMap;
    std::vector<coordinate> mFallingBytes;

    coordinate mPosition;
    coordinate mTargetPos;
    uint16_t mCounter = 0;

    coordinate mCriticalByte;
};
