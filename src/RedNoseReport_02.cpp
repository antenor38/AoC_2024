#include "RedNoseReport_02.hpp"

RedNoseReport::RedNoseReport(std::string filename)
  : IReaderConsumer(filename)
{

}

void RedNoseReport::parseData()
{
    for(auto& line : mFileLines)
    {
        std::vector<int> initialReport;
        std::set<int> report;
        auto reportString = mReader->splitString(line, " ");
        for(auto& levelString : reportString)
        {
            int level = mReader->stringToNumber(levelString);
            initialReport.push_back(level);
            report.insert(level);
        }
        mInitialReports.push_back(initialReport);
        mReports.push_back(report);
    }
}

int RedNoseReport::checkReports()
{
    int validReports = 0;

    for(size_t i = 0; i < mInitialReports.size(); i++)
    {
        if(checkValidity(mInitialReports.at(i), mReports.at(i)))
        {
            validReports++;
        }
    }

    return validReports;
}

bool RedNoseReport::checkElementsInOrderAndNoDuplicates(std::vector<int> originalReport, std::set<int> report) const
{
    return (std::equal(originalReport.begin(), originalReport.end(), report.begin(), report.end()) ||
       std::equal(originalReport.begin(), originalReport.end(), report.rbegin(), report.rend()));
}

bool RedNoseReport::checkValidity(std::vector<int> originalReport, std::set<int> report, bool tolerateSingleBadLevel)
{
    bool isValid = false;

    if((false == report.empty()) && checkElementsInOrderAndNoDuplicates(originalReport, report))
    {
        isValid = true;
        for(auto it1 = originalReport.begin(), it2 = ++originalReport.begin(); it2 != originalReport.end();)
        {
            if(std::abs(static_cast<int>(*it2) - static_cast<int>(*it1)) > 3)
            {
                if(tolerateSingleBadLevel && (originalReport.end() == (++it2)))
                {
                    isValid = true;
                    it2 = originalReport.erase(--it2);
                    tolerateSingleBadLevel = false;
                    break;
                }
                else if(tolerateSingleBadLevel && (originalReport.begin() == it1))
                {
                    tolerateSingleBadLevel = false;
                    it1 = originalReport.erase(it1);
                    it2 = it1 + 1;
                    continue;
                }
                else
                {
                    isValid = false;
                    break;
                }
            }
            it1++;
            it2++;
        }

        if(true == isValid)
        {
            return isValid;
        }
    }
    else if((false == report.empty()) && (1 >= (originalReport.size() - report.size())))
    {
        isValid = true;
        // first check for duplicate element. if present, erase it
        if(1 == originalReport.size() - report.size())
        {
            for(auto it = originalReport.begin(); it != originalReport.end();)
            {
                if((it + 1) != originalReport.end())
                {
                    if (*(it+1) == *(it))
                    {
                        if(tolerateSingleBadLevel)
                        {
                            tolerateSingleBadLevel = false;
                            it = originalReport.erase(it);
                            continue;
                        }
                        else
                        {
                            isValid = false;
                        }
                        break;
                    }
                }
                it++;
            }
        }
    }

    if(isValid || (true == report.empty()))
    {
        isValid = true;
        int previousSignum = 0;
        for(auto it1 = originalReport.begin(), it2 = originalReport.begin() + 1; it2 != originalReport.end();)
        {
            int diff = *(it1) - *(it2);
            uint absDiff = std::abs(diff);
            int signum = 0;
            if(0 != diff)
            {
                signum = diff / static_cast<int>(absDiff);
            }
            else
            {
                if(tolerateSingleBadLevel)
                {
                    tolerateSingleBadLevel = false;
                    originalReport.erase(it1);
                    isValid = checkValidity(originalReport, std::set<int>{}, tolerateSingleBadLevel);
                }
                else
                {
                    isValid = false;
                }
                break;
            }

            if((3 < absDiff) && (false == tolerateSingleBadLevel))
            {
                isValid = false;
                break;
            }

            int result = signum - previousSignum;

            // 1 - (-1) or -1 - 1 == |2|
            if(2 == std::abs(signum - previousSignum))
            {
                if(tolerateSingleBadLevel)
                {
                    tolerateSingleBadLevel = false;
                    auto index1 = std::distance(originalReport.begin(), it1);
                    auto index2 = std::distance(originalReport.begin(), it2);
                    std::vector<int> copyOriginalReport(originalReport);

                    auto copyIt2 = copyOriginalReport.begin() + index2;
                    copyOriginalReport.erase(copyIt2);
                    isValid = checkValidity(copyOriginalReport, std::set<int>{}, tolerateSingleBadLevel);
                    if((false == isValid) && (1 == index1))
                    {
                        copyOriginalReport = originalReport;
                        copyOriginalReport.erase(copyOriginalReport.begin());
                        isValid = checkValidity(copyOriginalReport, std::set<int>{}, tolerateSingleBadLevel);
                    }
                    if(false == isValid)
                    {
                        copyOriginalReport = originalReport;
                        auto copyIt1 = copyOriginalReport.begin() + index1;
                        copyOriginalReport.erase(copyIt1);
                        isValid = checkValidity(copyOriginalReport, std::set<int>{}, tolerateSingleBadLevel);
                    }

                    break;
                }
                else
                {
                    isValid  =false;
                    break;
                }
            }
            previousSignum = signum;
            it1++;
            it2++;
        }
    }

    return isValid;
}