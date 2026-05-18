#include "baseparser.h"
#include <cctype>
#include <stdexcept>
#include <vector>
#include <string>

static int oneEl(const string& s, size_t& pos, int base) {
    if (pos >= s.size()) {
        throw runtime_error("выход за строку");
    }
    if (s[pos] == '[') {
        size_t end = s.find(']', pos);
        if (end == string::npos) {
            throw runtime_error("нет закрывающей скобки");
        }
        string numStr = s.substr(pos + 1, end - pos - 1);
        if (numStr.empty()) {
            throw runtime_error("пустые скобки");
        }
        for (char c : numStr) {
            if (!std::isdigit(c)) {
                throw runtime_error("не цифра в квадратных скобках");
            }
        }
        int val = std::stoi(numStr);
        if (val >= base) {
            throw runtime_error("такого символа нет в системе счисления");
        }
        pos = end + 1;
        return val;
    }
    char c = s[pos];
    if (std::isdigit(c)) {
        int val = c - '0';
        if (val >= base) {
            throw runtime_error("такого символа нет в системе счисления");
        }
        ++pos;
        return val;
    }
    if (c >= 'A' && c <= 'Z') {
        int val = 10 + (c - 'A');
        if (val >= base) {
            throw runtime_error("такого символа нет в системе счисления");
        }
        ++pos;
        return val;
    }
    if (c >= 'a' && c <= 'z') {
        int val = 10 + (c - 'a');
        if (val >= base) {
            throw runtime_error("такого символа нет в системе счисления");
        }
        ++pos;
        return val;
    }
    throw runtime_error("!");
}

static vector<int> readDigits(const string& s, size_t& pos, int base, bool stopAtDotOrParen) {
    vector<int> res;
    while (pos < s.size()) {
        if (stopAtDotOrParen && (s[pos] == '.' || s[pos] == '(' || s[pos] == ')'))
            break;
        res.push_back(oneEl(s, pos, base));
    }
    if (res.empty() && stopAtDotOrParen)
        throw runtime_error("Expected at least one digit");
    return res;
}

static BigInteger buildNumber(const vector<int>& digits, int base) {
    BigInteger result(0);
    for (int d : digits) {
        result = result * base + BigInteger(d);
    }
    return result;
}

static BigInteger powBase(int base, size_t exp) {
    BigInteger result(1);
    for (size_t i = 0; i < exp; ++i)
        result = result * base;
    return result;
}


BigFraction BaseParser::parse(const std::string& input, int base) {
    size_t pos = 0;

    vector<int> intDigits = readDigits(input, pos, base, true);
    BigInteger I = buildNumber(intDigits, base);

    bool hasDot = (pos < input.size() && input[pos] == '.');
    if (hasDot) ++pos;

    vector<int> fracDigits;
    if (hasDot) {
        if (pos < input.size() && input[pos] != '(') {
            fracDigits = readDigits(input, pos, base, true);
        }
    }
    size_t k = fracDigits.size();
    BigInteger F = buildNumber(fracDigits, base);
    BigInteger p_pow_k = powBase(base, k);

    bool hasPeriod = (pos < input.size() && input[pos] == '(');
    if (hasPeriod) {
        ++pos;
        vector<int> periodDigits = readDigits(input, pos, base, false);
        if (periodDigits.empty())
            throw runtime_error("пустой период");
        if (pos >= input.size() || input[pos] != ')')
            throw runtime_error("неправильный период");
        ++pos;

        if (pos != input.size())
            throw runtime_error("лишний символ после периода");

        size_t m = periodDigits.size();
        BigInteger P = buildNumber(periodDigits, base);
        BigInteger p_pow_m = powBase(base, m);
        BigInteger p_pow_m_minus_1 = p_pow_m - BigInteger(1);

        BigInteger num = I * p_pow_k * p_pow_m_minus_1 + F * p_pow_m_minus_1 + P;
        BigInteger den = p_pow_k * p_pow_m_minus_1;
        return BigFraction(num, den);
    } else {
        if (pos != input.size())
            throw runtime_error("неправильный символ");
        BigInteger num = I * p_pow_k + F;
        return BigFraction(num, p_pow_k);
    }
}
