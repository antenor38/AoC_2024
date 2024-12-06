#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <bitset>

#include "Reader.hpp"

class XmasParser
{
public:
    XmasParser(std::string filename);

    uint16_t countWords() const;

private:
    uint16_t searchWord(std::string word, uint16_t linePos, uint16_t charPos, uint8_t paths = 0b111) const;
    bool searchXWord(uint16_t linePos, uint16_t charPos) const;

    std::vector<std::string> mFileLines;
    std::unique_ptr<Reader> mReader;
    std::string const kWord;
};