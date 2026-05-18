#ifndef BASE_PARSER_H
#define BASE_PARSER_H

#include "bigfraction.h"
#include <string>

class BaseParser {
public:
    static BigFraction parse(const string& input, int base);
};

#endif
