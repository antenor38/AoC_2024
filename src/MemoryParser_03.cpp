#include "MemoryParser_03.hpp"

MemoryParser::MemoryParser(std::string filename)
  : IReaderConsumer(filename)
{

}

void MemoryParser::parseData()
{
    for(auto line : mFileLines)
    {
        auto multiplicationCandidates = mReader->splitString(line, std::string(kDelimiter));


        for(auto& candidate : multiplicationCandidates)
        {
            uint64_t firstOp, secondOp;

            if(mReader->parseUInt(candidate, firstOp, ','))
            {
                std::string secondPartCandidate = mReader->splitString(candidate, ",").at(1);
                if(mReader->parseUInt(secondPartCandidate, secondOp, ')'))
                {
                    mProducts.push_back(firstOp * secondOp);
                }
            }
        }
    }
}

uint64_t MemoryParser::getResult() const
{
    return std::reduce(mProducts.begin(), mProducts.end());
}