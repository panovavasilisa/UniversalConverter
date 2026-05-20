#ifndef BASECONVERTER_H
#define BASECONVERTER_H

#include "bigfraction.h"
#include <string>
#include <utility>

class BaseConverter {
public:
    static const size_t MAX_OUTPUT_LENGTH = 100000;

    static std::pair<std::string, std::string> convert(const BigFraction& fraction, int base);

private:
    static string digitToString(int digit);
};

#endif
