#include <iostream>
#include <string>
#include <map>
#include <set>
#include <memory>

#include "Reader.hpp"

typedef bool obstruction;
typedef std::pair<uint16_t, uint16_t> coordinate;

class LabGrid
{
public:
    LabGrid(std::string filename);

    uint16_t calculateDistinctPositions();

    uint16_t getNumObstacles() const
    {
        return mNumObstacles;
    }

private:
    void populateGrid();

    uint16_t* getDirectionPtr(uint8_t direction, int16_t& oper, coordinate* position);

    void moveGuard(uint8_t direction, bool fakePath = false);

    bool checkObstructionCandidate(uint8_t direction, uint16_t* axisPtr, int16_t oper, coordinate position);

    struct posCompare {
        bool operator()(coordinate const& a, coordinate const& b)const {
            return (a.first < b.first) || ((a.first == b.first) && (a.second < b.second));
        }
    };

    std::map<coordinate, obstruction, posCompare> mLabGrid;
    std::map<coordinate, std::set<uint8_t>> mGuardPath;
    std::map<coordinate, std::set<uint8_t>> mFakeGuardPath;
    std::unique_ptr<Reader> mReader;
    uint16_t mMaxX, mMaxY;
    coordinate mGuardPos;
    std::vector<std::string> mFileLines;
    uint16_t mNumObstacles;
};