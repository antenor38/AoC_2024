#include "GardenPlots_12.hpp"

Plot::Plot(char const plantType)
  : mPlantType(plantType)
  , mArea(0)
  , mPerimeter(0)
  , mSides(0)
{

}

void Plot::addPlant(uint16_t perimeter)
{
    mArea++;
    mPerimeter += perimeter;
}

void Plot::addCoordinate(coordinate pos)
{
    mSideCoordinates.push_back(pos);
}

void Plot::addCoordinate(std::vector<coordinate> coordinates)
{
    mSideCoordinates.insert(mSideCoordinates.end(), coordinates.begin(), coordinates.end());
}

// void Plot::addYCoordinate(coordinate pos)
// {
//     mYCoordinates.push_back(pos);
// }

// void Plot::addYCoordinate(std::vector<coordinate> coordinates)
// {
//     mYCoordinates.insert(mYCoordinates.end(), coordinates.begin(), coordinates.end());
// }

Plot& Plot::operator+=(Plot const& rhs)
{
    if(this->mPlantType == rhs.mPlantType)
    {
        this->mArea += rhs.mArea;
        this->mPerimeter += rhs.mPerimeter;
        this->mSideCoordinates.insert(mSideCoordinates.end(), rhs.mSideCoordinates.begin(), rhs.mSideCoordinates.end());
    }
    return *this;
}

uint16_t Plot::calculateSides()
{
    bool newSide = true;
    for(auto it = mSideCoordinates.begin(); it != mSideCoordinates.end(); it++)
    {
        bool skip = false;
        coordinate pos = *it;
        uint16_t x = 0,y = 0;
        if(true == newSide)
        {
            x = pos.first;
            y = pos.second;
            newSide = false;
            mSides++;
        }
        else if((x == pos.first) && (y == pos.second))
        {
            mSides++;
        }
        else if((x != pos.first) && (y != pos.second))
        {
            mSides++;
            x = pos.first;
            y = pos.second;
        }
        else if (x != pos.first)
        {
            x = pos.first;
        }
        else if (y != pos.second)
        {
            y = pos.second;
        }
        // std::cout << mPlantType << ": (" << pos.first << "," << pos.second << ") mSides = " << mSides << " | ";
    }

    std::cout << "\n";

    return mSides;
}

uint32_t Plot::fencePrice(bool bulkVersion)
{
    if(true == bulkVersion)
    {
        if(0 == mSides)
        {
            calculateSides();
        }
        std::cout << mArea << " * " << mSides << " ";
        return static_cast<uint32_t>(mArea) * static_cast<uint32_t>(mSides);
    }
    return static_cast<uint32_t>(mArea) * static_cast<uint32_t>(mPerimeter);
}


GardenPlots::GardenPlots(std::string const filename)
  : mReader(std::make_unique<Reader>(filename))
{
    mFileLines = mReader->readLines();
    mLengthX = mFileLines.at(0).size();
    mLengthY = mFileLines.size();
}

void GardenPlots::parseField()
{
    for(uint16_t y = 0; y < mLengthY; y++)
    {
        std::string const line = mFileLines.at(y);
        for(uint16_t x = 0; x < mLengthX; x++)
        {
            std::vector<coordinate> sideCoordinates;
            // std::set<coordinate> sideYCoordinates;
            char const c = line.at(x);
            uint16_t perimeter = 0;
            std::shared_ptr<Plot> plot(nullptr);

            if((((x + 1) < mLengthX) && (c != line.at(x+1))) || ((x + 1) == mLengthX))
            {
                perimeter++;
                // sideCoordinates.push_back(coordinate(x,y));
            }
            if((((y + 1) < mLengthY) && (c != mFileLines.at(y+1).at(x))) || ((y + 1) == mLengthY))
            {
                perimeter++;
                // sideCoordinates.push_back(coordinate(x,y));
            }
            if(0 != x)
            {
                if(c == line.at(x-1))
                {
                    if(mPlotsMap.end() != mPlotsMap.find(coordinate(x-1, y)))
                    {
                        plot = mPlotsMap[coordinate(x-1,y)];
                    }
                    else
                    {
                        std::cerr << "Expected to find a plot on coordinates: " << x-1 << "," << y << std::endl;
                    }
                }
                else
                {
                    perimeter++;
                    // sideCoordinates.push_back(coordinate(x,y));
                }
            }
            else
            {
                perimeter++;
                // sideCoordinates.push_back(coordinate(x,y));
            }
            if(0 != y)
            {
                if(c == mFileLines.at(y-1).at(x))
                {
                    if(mPlotsMap.end() != mPlotsMap.find(coordinate(x, y-1)))
                    {
                        if(nullptr == plot)
                        {
                            plot = mPlotsMap[coordinate(x,y-1)];
                        }
                        else if(plot != mPlotsMap[coordinate(x,y-1)]) // two parts of the same plot, have to be merged
                        {
                            // std::cout << "Plant " << c << " at coordinates " << x << "," << y << "has 2 plots\n";
                            std::shared_ptr<Plot> otherPlot = mPlotsMap[coordinate(x,y-1)];
                            (*plot) += (*otherPlot);
                            auto const coordinates = mReversePlotsMap[otherPlot];
                            for(auto const& position : coordinates)
                            {
                                mPlotsMap[position] = plot;
                                mReversePlotsMap[plot].push_back(position);
                            }
                            mReversePlotsMap.erase(otherPlot);
                            mPlotsList.erase(otherPlot);
                            otherPlot.reset();
                        }
                    }
                    else
                    {
                        std::cerr << "Expected to find a plot on coordinates: " << x << "," << y-1 << std::endl;
                    }
                }
                else
                {
                    perimeter++;
                    // sideCoordinates.push_back(coordinate(x,y));
                }
            }
            else
            {
                perimeter++;
                // sideCoordinates.push_back(coordinate(x,y));
            }

            if(nullptr == plot)
            {
                plot = std::make_shared<Plot>(c);
            }
            plot->addPlant(perimeter);
            if(0 != perimeter)
            {
                plot->addCoordinate(coordinate(x,y));
            }
            mPlotsList.insert(plot);
            mPlotsMap[coordinate(x,y)] = plot;
            mReversePlotsMap[plot].push_back(coordinate(x,y));
        }
    }
}

uint32_t GardenPlots::totalFencePrice() const
{
    uint32_t sum = 0;
    std::cout << "\n";
    for(auto plot : mPlotsList)
    {
        std::cout << "Plot: " << plot->getPlantType() << " with price " << plot->fencePrice() << std::endl; 
        sum += plot->fencePrice();
    }

    return sum;
}