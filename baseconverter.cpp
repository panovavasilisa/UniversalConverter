#include "baseconverter.h"
#include "biginteger.h"
#include "bigfraction.h"
#include <unordered_map>

std::string BaseConverter::digitToString(int digit) {
    if (digit < 10) return std::string(1, '0' + digit);
    if (digit <= 35) return std::string(1, 'A' + (digit - 10));
    return "[" + std::to_string(digit) + "]";
}

std::pair<std::string, std::string> BaseConverter::convert(const BigFraction& fraction, int base) {
    const BigInteger& a = fraction.GetNumerator();
    const BigInteger& b = fraction.GetDenominator();

    BigInteger intPart = a / b;
    BigInteger remainder = a % b;

    string intStr;
    if (intPart.isZero()) {
        intStr = "0";
    } else {
        BigInteger tmp = intPart;
        while (!tmp.isZero()) {
            unsigned int digit = tmp % base;
            tmp = tmp / base;
            intStr = digitToString(digit) + intStr;
        }
    }

    std::vector<int> fracDigits;
    std::unordered_map<std::string, size_t> seenRemainders;
    bool periodFound = false;
    size_t periodStart = 0;
    BigInteger currentRem = remainder;

    while (!currentRem.isZero() && fracDigits.size() < MAX_OUTPUT_LENGTH) {
        std::string key = currentRem.toString();
        if (seenRemainders.find(key) != seenRemainders.end()) {
            periodFound = true;
            periodStart = seenRemainders[key];
            break;
        }
        seenRemainders[key] = fracDigits.size();

        currentRem = currentRem * base;
        BigInteger digitBig = currentRem / b;
        unsigned int digit = digitBig % base;
        fracDigits.push_back(static_cast<int>(digit));
        currentRem = currentRem % b;
    }

    // Формирование строки дробной части
    std::string fracStr;
    bool truncated = false;
    if (fracDigits.size() >= MAX_OUTPUT_LENGTH && !currentRem.isZero()) {
        truncated = true;
        for (size_t i = 0; i < MAX_OUTPUT_LENGTH; ++i)
            fracStr += digitToString(fracDigits[i]);
    } else if (periodFound) {
        for (size_t i = 0; i < periodStart; ++i)
            fracStr += digitToString(fracDigits[i]);
        fracStr += '(';
        for (size_t i = periodStart; i < fracDigits.size(); ++i)
            fracStr += digitToString(fracDigits[i]);
        fracStr += ')';
    } else {
        for (int d : fracDigits)
            fracStr += digitToString(d);
    }

    // Сборка результата
    std::string result = intStr;
    if (!fracStr.empty())
        result += "." + fracStr;

    std::string message;
    if (truncated) {
        message = "Полная запись результата слишком длинная. Показаны первые " +
                  std::to_string(MAX_OUTPUT_LENGTH) + " символов.";
    }
    return {result, message};
}
