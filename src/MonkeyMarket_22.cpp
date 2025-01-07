#include "MonkeyMarket_22.hpp"

MonkeyBuyer::MonkeyBuyer(uint64_t seed)
  : mSeed(seed)
{

}

std::unordered_map<SequenceQueue<int, 4>, uint64_t, SequenceHasher<int, 4>> MonkeyBuyer::calculateSequencePrices(uint16_t steps)
{
    uint64_t max = 0;
    SequenceQueue<int, 4> currentSequence;
    while(steps > 0)
    {
        steps--;
        int oldOffer = mSeed % 10;
        calculateFirstStep();
        calculateSecondStep();
        calculateThirdStep();
        int newOffer = mSeed % 10;
        int diff = newOffer - oldOffer;
        currentSequence.push(diff);
        if((4 == currentSequence.size()) && (0 == mSequenceCombinations.count(currentSequence)))
        {
            mSequenceCombinations[currentSequence] = newOffer;
        }
        // std::cout << "Num: " << mSeed << std::endl;
    }
    return mSequenceCombinations;
}
void MonkeyBuyer::calculateFirstStep()
{
    auto tempResult = mSeed * 64;
    mSeed = mSeed ^ tempResult;
    mSeed = mSeed % 16777216;
}

void MonkeyBuyer::calculateSecondStep()
{
    auto tempResult = mSeed / 32;
    mSeed = mSeed ^ tempResult;
    mSeed = mSeed % 16777216;
}

void MonkeyBuyer::calculateThirdStep()
{
    auto tempResult = mSeed * 2048;
    mSeed = mSeed ^ tempResult;
    mSeed = mSeed % 16777216;
}

MonkeyMarket::MonkeyMarket(std::string filename)
  : mReader(std::make_unique<Reader>(filename))
{
    mFileLines = mReader->readLines();
}

void MonkeyMarket::parseData()
{
    for(auto line : mFileLines)
    {
        uint64_t num = mReader->stringToNumber(line);
        mMonkeys.push_back(MonkeyBuyer(num));
    }
}

uint64_t MonkeyMarket::calculateBestPriceBySequence()
{
    uint64_t max = 0;
    for(auto& monkey : mMonkeys)
    {
        auto const monkeyMap = monkey.calculateSequencePrices(2000);
        for(auto const& elem : monkeyMap)
        {
            mSequenceCombinations[elem.first] += elem.second;
        }
    }

    for(auto const& elem : mSequenceCombinations)
    {
        if(elem.second > max)
        {
            max = elem.second;
        }
    }

    return max;
}