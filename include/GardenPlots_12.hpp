#include <iostream>
#include <string>
#include <map>
#include <set>
#include <memory>

#include "Reader.hpp"

typedef std::pair<uint16_t, uint16_t> coordinate;

struct BorderCoordinate
{
    coordinate pos;
    bool left = false;
    bool up = false;
    bool right = false;
    bool down = false;

    BorderCoordinate* next = nullptr;
};

class Plot
{
public:
    Plot(char const plantType);
    ~Plot()
    {
        deleteBorderElems();
    }

    char getPlantType() const {return mPlantType;}

    void addPlant(uint16_t perimeter);
    void addCoordinate(coordinate pos);
    void addCoordinate(std::vector<coordinate> coordinates);

    Plot& operator+=(Plot const& rhs);

    uint16_t calculateSides();

    uint32_t fencePrice(bool bulkVersion = false);

    BorderCoordinate* getLastBorderElem();
    void addBorderElem(BorderCoordinate* borCoor);
    void deleteBorderElems();

private:

    struct posCompareX {
        bool operator()(coordinate const& a, coordinate const& b)const {
            return (a.first < b.first) || ((a.first == b.first) && (a.second < b.second));
        }
    };

    struct posCompareY {
        bool operator()(coordinate const& a, coordinate const& b)const {
            return (a.second < b.second) || ((a.second == b.second) && (a.first < b.first));
        }
    };

    char mPlantType;
    uint16_t mArea;
    uint16_t mPerimeter;
    uint16_t mSides;

    std::vector<coordinate> mSideCoordinates; 
    BorderCoordinate* mBorderCoordinate;
};

class GardenPlots
{
public:
    GardenPlots(std::string const filename);

    void parseField();

    uint32_t totalFencePrice() const;

private:
    std::unique_ptr<Reader> mReader;
    std::vector<std::string> mFileLines;
    
    uint16_t mLengthX, mLengthY;
    std::map<coordinate, std::shared_ptr<Plot>> mPlotsMap;
    std::map<std::shared_ptr<Plot>, std::vector<coordinate>> mReversePlotsMap;
    std::set<std::shared_ptr<Plot>> mPlotsList;
};