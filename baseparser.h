#ifndef BASEPARSER_H
#define BASEPARSER_H

#include "bigfraction.h"
#include <string>

class BaseParser {
public:
    static BigFraction parse(const std::string& input, int base);
};

#endif
