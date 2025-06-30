#include <iostream>
#include <string>
#include <memory>
#include <set>
#include <algorithm>
#include <cmath>

#include "Reader.hpp"



class RedNoseReport : IReaderConsumer
{
public:
    RedNoseReport(std::string filename);

    void parseData() override;

    int checkReports();

private:

    bool checkValidity(std::vector<int> originalReport, std::set<int> report, bool tolerateSingleBadLevel = true);

    bool checkElementsInOrderAndNoDuplicates(std::vector<int> originalReport, std::set<int> report) const;

    std::vector<std::vector<int>> mInitialReports;
    std::vector<std::set<int>> mReports;

    static int constexpr kMaxDiff = 3;
};