#include "baseparser.h"
#include <cctype>
#include <stdexcept>
#include <vector>
#include <string>

using namespace std;

static string posErr(size_t pos) {
    return "в позиции " + to_string(pos + 1);
}

static int readOneDigit(const string& s, size_t& i, int base) {
    if (i >= s.size()) throw runtime_error("неожиданный конец строки");
    if (s[i] == '[') {
        size_t j = s.find(']', i);
        if (j == string::npos) throw runtime_error("незакрытая квадратная скобка");
        string numStr = s.substr(i + 1, j - i - 1);
        if (numStr.empty()) throw runtime_error("пустые квадратные скобки");
        for (char c : numStr) {
            if (!isdigit(static_cast<unsigned char>(c)))   // ← исправлено
                throw runtime_error("в скобках не цифра");
        }
        int val = stoi(numStr);
        if (val >= base) throw runtime_error("значение цифры больше или равно основанию");
        i = j + 1;
        return val;
    }
    char c = s[i];
    if (isdigit(static_cast<unsigned char>(c))) {
        int val = c - '0';
        if (val >= base) throw runtime_error("цифра больше или равна основанию");
        ++i;
        return val;
    }
    if (c >= 'A' && c <= 'Z') {
        int val = 10 + (c - 'A');
        if (val >= base) throw runtime_error("буквенная цифра больше или равна основанию");
        ++i;
        return val;
    }
    if (c >= 'a' && c <= 'z') {
        int val = 10 + (c - 'a');
        if (val >= base) throw runtime_error("буквенная цифра больше или равна основанию");
        ++i;
        return val;
    }
    throw runtime_error("недопустимый символ");
}

static vector<int> readDigits(const string& s, size_t& i, int base, bool stopAtDotOrParen) {
    vector<int> res;
    while (i < s.size()) {
        if (stopAtDotOrParen && (s[i] == '.' || s[i] == '(' || s[i] == ')'))
            break;
        res.push_back(readOneDigit(s, i, base));
    }
    if (res.empty() && stopAtDotOrParen)
        throw runtime_error("ожидалась хотя бы одна цифра");
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

BigFraction BaseParser::parse(const string& input, int base) {
    size_t i = 0;

    vector<int> intDigits = readDigits(input, i, base, true);
    BigInteger I = buildNumber(intDigits, base);

    bool hasDot = (i < input.size() && input[i] == '.');
    if (hasDot) ++i;

    vector<int> fracDigits;
    if (hasDot) {
        if (i < input.size() && input[i] != '(')
            fracDigits = readDigits(input, i, base, true);
    }
    size_t k = fracDigits.size();
    BigInteger F = buildNumber(fracDigits, base);
    BigInteger p_pow_k = powBase(base, k);

    bool hasPeriod = (i < input.size() && input[i] == '(');
    if (hasPeriod && !hasDot) {
        throw runtime_error("Период может быть только после точки");
    }
    if (hasDot && fracDigits.empty() && !hasPeriod) {
        throw runtime_error("После точки нет ни цифр, ни периода");
    }

    if (hasPeriod) {
        ++i;
        vector<int> periodDigits = readDigits(input, i, base, true);
        if (periodDigits.empty())
            throw runtime_error("пустой период");
        if (i >= input.size() || input[i] != ')')
            throw runtime_error("нет закрывающей скобки периода");
        ++i;
        if (i != input.size())
            throw runtime_error("лишние символы после периода");

        size_t m = periodDigits.size();
        BigInteger P = buildNumber(periodDigits, base);
        BigInteger p_pow_m = powBase(base, m);
        BigInteger p_pow_m_minus_1 = p_pow_m - BigInteger(1);

        BigInteger num = I * p_pow_k * p_pow_m_minus_1 + F * p_pow_m_minus_1 + P;
        BigInteger den = p_pow_k * p_pow_m_minus_1;
        return BigFraction(num, den);
    } else {
        if (i != input.size())
            throw runtime_error("неожиданные символы после дробной части");
        BigInteger num = I * p_pow_k + F;
        return BigFraction(num, p_pow_k);
    }
}
