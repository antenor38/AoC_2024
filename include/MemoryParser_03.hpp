#include <iostream>
#include <string>
#include <string_view>
#include <memory>
#include <set>
#include <algorithm>
#include <cmath>
#include <numeric>

#include "Reader.hpp"



class MemoryParser : IReaderConsumer
{
public:
    MemoryParser(std::string filename);

    void parseData() override;

    uint64_t getResult() const;

private:

    uint32_t parseMultiplicationCandidate(std::string& candidate);

    std::vector<uint64_t> mProducts;

    static std::string_view constexpr kDelimiter = "mul(";
};