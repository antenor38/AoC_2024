#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <memory>
#include <algorithm>

#include "Reader.hpp"

template<typename T, size_t maxLen>
class SequenceQueue : public std::vector<T>
{
public:
    void push(const T& value)
    {
        
        if(this->size() == maxLen)
        {
            this->erase(this->begin());
        }
        this->push_back(value);
    }

    bool operator==(SequenceQueue<T, maxLen> const& elem)
    {
        size_t result = 0;

        if(this->size() == elem.size())
        {
            for(size_t i = 0; i < this->size(); i++)
            {
                if(this->at(i) != elem.at(i))
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
};

template<typename T, size_t maxLen>
struct SequenceHasher
{
  std::size_t operator()(const SequenceQueue<T, maxLen>& k) const
  {
    using std::size_t;
    using std::hash;
    using std::string;

    size_t returnVal = 0;

    for(auto it = k.begin(); it != k.end(); it++)
    {
        returnVal ^= hash<T>()((*it) | returnVal);
        if(k.begin() == it)
        {
            returnVal >>= 2;
        }
    }

    return returnVal;
  }
};

class MonkeyBuyer
{
public:
    MonkeyBuyer(uint64_t seed);

    std::unordered_map<SequenceQueue<int, 4>, uint64_t, SequenceHasher<int, 4>> calculateSequencePrices(uint16_t steps = 1);

private:
    void calculateFirstStep();
    void calculateSecondStep();
    void calculateThirdStep();

    std::unordered_map<SequenceQueue<int, 4>, uint64_t, SequenceHasher<int, 4>> mSequenceCombinations;
    uint64_t mSeed;
};

class MonkeyMarket
{
public:
    MonkeyMarket(std::string filename);

    void parseData();

    uint64_t calculateBestPriceBySequence();

private:
    std::unique_ptr<Reader> mReader;
    std::vector<std::string> mFileLines;

    std::vector<MonkeyBuyer> mMonkeys;
    std::unordered_map<SequenceQueue<int, 4>, uint64_t, SequenceHasher<int, 4>> mSequenceCombinations;
};