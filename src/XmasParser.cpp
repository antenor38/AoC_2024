#include "XmasParser.hpp"

XmasParser::XmasParser(std::string filename)
  : mReader(std::make_unique<Reader>(filename))
  , kWord("XMAS")
{
    mFileLines = mReader->readLines();
}

uint16_t XmasParser::countWords() const
{
    uint16_t result = 0;

    for(uint16_t linePos = 1; linePos < mFileLines.size() - 1; linePos++)
    {
        std::string const line = mFileLines.at(linePos);
        for(uint16_t charPos = 1; charPos < line.length() - 1; charPos++)
        {
            char const c = line.at(charPos);
            if(c == 'A')
            {
                if(true == searchXWord(linePos, charPos))
                {
                    result++;
                }
            }

        }
    }

    return result;
}

uint16_t XmasParser::searchWord(std::string word, uint16_t linePos, uint16_t charPos, uint8_t paths) const
{
    uint16_t result = 0;

    if(mFileLines.size() <= linePos)
    {
        return 0;
    }

    std::string const line = mFileLines.at(linePos);
    char const c = word.front();

    if((0b100 == (paths & 0b100)) && (0 < charPos))
    {
        if(c == line.at(charPos - 1))
        {
            if(1 == word.length())
            {
                return 1;
            }
            result += searchWord(word.substr(1), linePos + 1, charPos - 1, 0b100);
        }
    }

    if(0b010 == (paths & 0b010))
    {
        if(c == line.at(charPos))
        {
            if(1 == word.length())
            {
                return 1;
            }

            result += searchWord(word.substr(1), linePos + 1, charPos, 0b010);
        }
    }
        
    if((0b001 == (paths & 0b001)) && (++charPos < line.length()))
    {
        if(c == line.at(charPos))
        {
            if(1 == word.length())
            {
                return 1;
            }

            result += searchWord(word.substr(1), linePos + 1, charPos, 0b001);
        }
    }

    return result;
}

bool XmasParser::searchXWord(uint16_t linePos, uint16_t charPos) const
{
    std::string lineAbove = mFileLines.at(linePos-1);
    std::string lineBelow = mFileLines.at(linePos+1);

    if('M' == lineAbove.at(charPos-1))
    {
        if('S' != lineBelow.at(charPos+1))
        {
            return false;
        }
    }
    else if('S' == lineAbove.at(charPos-1))
    {
        if('M' != lineBelow.at(charPos+1))
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    if('M' == lineAbove.at(charPos+1))
    {
        if('S' != lineBelow.at(charPos-1))
        {
            return false;
        }
    }
    else if('S' == lineAbove.at(charPos+1))
    {
        if('M' != lineBelow.at(charPos-1))
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}
