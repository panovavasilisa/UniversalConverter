#include "biginteger.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <cstdint>
using namespace std;

const int BigInteger::BASE;

BigInteger::BigInteger() {
    number.push_back(0);
}

BigInteger::BigInteger(int num) {
    do {
        number.push_back(num % BASE);
        num /= BASE;
    } while (num > 0);
}

BigInteger::BigInteger(string num) {
    if (num.empty() || (num.size() == 1 && num[0] == '0')) {
        number.push_back(0);
        return;
    }
    size_t pos = num.size();
    while (pos > 0) {
        size_t len = (pos >= 9) ? 9 : pos;
        pos -= len;
        string part = num.substr(pos, len);
        int val = stoi(part);
        number.push_back(val);
    }
    trim();
}

void BigInteger::trim() {
    while (number.size() > 1 && number.back() == 0) {
        number.pop_back();
    }
}

bool BigInteger::isZero() const {
    return (number.size() == 1 && number[0] == 0);
}

string BigInteger::toString() {
    if (isZero()) return "0";
    string res;
    res += to_string(number.back());
    for (int i = (int)number.size() - 2; i >= 0; --i) {
        string part = to_string(number[i]);
        part.insert(part.begin(), 9 - part.size(), '0');
        res += part;
    }
    return res;
}

bool BigInteger::operator==(const BigInteger& other) const{
    if (number.size() != other.number.size()) return false;
    for (size_t i = 0; i < number.size(); ++i) {
        if (number[i] != other.number[i]) return false;
    }
    return true;
}

bool BigInteger::operator!=(const BigInteger& other) const{
    return !(*this == other);
}

bool BigInteger::operator<(const BigInteger& other) const{
    if (number.size() != other.number.size())
        return number.size() < other.number.size();
    for (int i = (int)number.size() - 1; i >= 0; --i) {
        if (number[i] != other.number[i])
            return number[i] < other.number[i];
    }
    return false;
}

bool BigInteger::operator<=(const BigInteger& other) const{
    return (*this < other) || (*this == other);
}

bool BigInteger::operator>(const BigInteger& other) const{
    return !(*this <= other);
}

bool BigInteger::operator>=(const BigInteger& other) const{
    return !(*this < other);
}

BigInteger& BigInteger::operator+=(const BigInteger& other) {
    size_t maxSize = max(number.size(), other.number.size());
    number.resize(maxSize + 1, 0);
    uint64_t carry = 0;
    for (size_t i = 0; i < maxSize; ++i) {
        uint64_t sum = carry;
        if (i < number.size()) sum += number[i];
        if (i < other.number.size()) sum += other.number[i];
        number[i] = static_cast<int>(sum % BASE);
        carry = sum / BASE;
    }
    if (carry) {
        number[maxSize] = static_cast<int>(carry);
    } else {
        number.resize(maxSize);
    }
    trim();
    return *this;
}

BigInteger BigInteger::operator+(const BigInteger& other) const{
    BigInteger result = *this;
    result += other;
    return result;
}

BigInteger& BigInteger::operator-=(const BigInteger& other) {
    if (number.size() < other.number.size()) {
        throw runtime_error("вычитаемое число больше!!!");
    }
    if (number.size() == other.number.size() && number.back() < other.number.back()) {
        throw runtime_error("вычитаемое число больше!!!");
    }
    int fl = 0;
    for (size_t i = 0; i < number.size(); ++i) {
        int64_t diff = static_cast<int64_t>(number[i]) - fl;
        if (i < other.number.size()) diff -= other.number[i];
        if (diff < 0) {
            diff += BASE;
            fl = 1;
        } else {
            fl = 0;
        }
        number[i] = static_cast<int>(diff);
    }
    trim();
    return *this;
}

BigInteger BigInteger::operator-(const BigInteger& other) const{
    BigInteger result = *this;
    result -= other;
    return result;
}

BigInteger& BigInteger::operator*=(unsigned int x) {
    if (x == 0) {
        number = {0};
        return *this;
    }
    uint64_t carry = 0;
    for (size_t i = 0; i < number.size(); ++i) {
        uint64_t prod = static_cast<uint64_t>(number[i]) * x + carry;
        number[i] = static_cast<int>(prod % BASE);
        carry = prod / BASE;
    }
    if (carry) {
        number.push_back(static_cast<int>(carry));
    }
    trim();
    return *this;
}

BigInteger BigInteger::operator*(unsigned int x) const{
    BigInteger result = *this;
    result *= x;
    return result;
}
//проверка парсера
std::vector<long long> BigInteger::karatsuba(const std::vector<int>& a, const std::vector<int>& b) {
    size_t n = std::max(a.size(), b.size());
    if (n <= 64) {
        std::vector<long long> res(2 * n, 0);
        for (size_t i = 0; i < a.size(); ++i) {
            for (size_t j = 0; j < b.size(); ++j) {
                res[i + j] += (long long)a[i] * b[j];
            }
        }
        long long carry = 0;
        for (size_t i = 0; i < res.size(); ++i) {
            res[i] += carry;
            carry = res[i] / BASE;
            res[i] %= BASE;
        }
        while (carry) {
            res.push_back(carry % BASE);
            carry /= BASE;
        }
        return res;
    }
    size_t k = n / 2;
    std::vector<int> a_low(a.begin(), a.begin() + std::min(k, a.size()));
    std::vector<int> a_high(a.begin() + std::min(k, a.size()), a.end());
    std::vector<int> b_low(b.begin(), b.begin() + std::min(k, b.size()));
    std::vector<int> b_high(b.begin() + std::min(k, b.size()), b.end());

    a_low.resize(k, 0);
    a_high.resize(k, 0);
    b_low.resize(k, 0);
    b_high.resize(k, 0);

    std::vector<long long> ac = karatsuba(a_high, b_high);
    std::vector<long long> bd = karatsuba(a_low, b_low);

    std::vector<int> a_sum(k), b_sum(k);
    for (size_t i = 0; i < k; ++i) {
        a_sum[i] = a_low[i] + a_high[i];
        b_sum[i] = b_low[i] + b_high[i];
    }

    int carry = 0;
    for (size_t i = 0; i < k; ++i) {
        a_sum[i] += carry;
        carry = a_sum[i] / BASE;
        a_sum[i] %= BASE;
    }
    if (carry) a_sum.push_back(carry);
    carry = 0;
    for (size_t i = 0; i < k; ++i) {
        b_sum[i] += carry;
        carry = b_sum[i] / BASE;
        b_sum[i] %= BASE;
    }
    if (carry) b_sum.push_back(carry);

    std::vector<long long> sum_prod = karatsuba(a_sum, b_sum);

    size_t max_size = std::max(std::max(bd.size(), sum_prod.size()), ac.size());
    std::vector<long long> result(max_size + 2 * k, 0);

    for (size_t i = 0; i < bd.size(); ++i) result[i] = bd[i];
    for (size_t i = 0; i < ac.size(); ++i) result[i + 2 * k] += ac[i];
    for (size_t i = 0; i < sum_prod.size(); ++i) {
        long long val = sum_prod[i];
        if (i < ac.size()) val -= ac[i];
        if (i < bd.size()) val -= bd[i];
        result[i + k] += val;
    }

    long long carry2 = 0;
    for (size_t i = 0; i < result.size(); ++i) {
        result[i] += carry2;
        carry2 = result[i] / BASE;
        result[i] %= BASE;
    }
    while (carry2) {
        result.push_back(carry2 % BASE);
        carry2 /= BASE;
    }
    while (result.size() > 1 && result.back() == 0) result.pop_back();
    return result;
}

BigInteger& BigInteger::operator*=(const BigInteger& other) {
    if (isZero() || other.isZero()) {
        *this = BigInteger(0);
        return *this;
    }
    std::vector<long long> res = karatsuba(number, other.number);
    number.clear();
    number.reserve(res.size());
    for (long long v : res) number.push_back(static_cast<int>(v));
    trim();
    return *this;
}

/*BigInteger& BigInteger::operator*=(const BigInteger& other) {
    if (isZero() || other.isZero()) {
        *this = BigInteger(0);
        return *this;
    }
    vector<long long> tmp(number.size() + other.number.size(), 0);
    for (size_t i = 0; i < number.size(); ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < other.number.size(); ++j) {
            uint64_t prod = (uint64_t)number[i] * other.number[j] + tmp[i + j] + carry;
            tmp[i + j] = prod % BASE;
            carry = prod / BASE;
        }
        if (carry) tmp[i + other.number.size()] += carry;
    }
    uint64_t carry = 0;
    for (size_t i = 0; i < tmp.size(); ++i) {
        tmp[i] += carry;
        carry = tmp[i] / BASE;
        tmp[i] %= BASE;
    }
    while (carry) {
        tmp.push_back(carry % BASE);
        carry /= BASE;
    }
    number.clear();
    number.reserve(tmp.size());
    for (long long v : tmp) number.push_back((int)v);
    trim();
    return *this;
}*/

BigInteger BigInteger::operator*(const BigInteger& other) const{
    BigInteger result = *this;
    result *= other;
    return result;
}

BigInteger BigInteger::operator/(unsigned int x) const{
    BigInteger result = *this;
    result /= x;
    return result;
}

BigInteger& BigInteger::operator/=(unsigned int x) {
    if (x == 0) throw runtime_error("деление на 0");
    if (isZero()) return *this;
    uint64_t rem = 0;
    for (int i = (int)number.size() - 1; i >= 0; --i) {
        uint64_t cur = rem * BASE + number[i];
        number[i] = static_cast<int>(cur / x);
        rem = cur % x;
    }
    trim();
    return *this;
}

unsigned int BigInteger::operator%(unsigned int x) const{
    if (x == 0) throw runtime_error("деление на 0");
    uint64_t rem = 0;
    for (int i = (int)number.size() - 1; i >= 0; --i) {
        rem = (rem * BASE + number[i]) % x;
    }
    return static_cast<unsigned int>(rem);
}

/*BigInteger BigInteger::operator/(const BigInteger& other) const{
    if (other.isZero()) throw runtime_error("Division by zero");
    if (isZero()) return BigInteger(0);
    if (*this < other) return BigInteger(0);
    if (other == BigInteger(1)) return *this;
    int d = BASE / (other.number.back() + 1);
    BigInteger U = *this;
    BigInteger V = other;
    U *= d;
    V *= d;
    size_t n = V.number.size();
    size_t m = U.number.size() - n;
    vector<int> q(m + 1, 0);
    for (size_t j = m; j + 1 > 0; --j) {
        size_t idx = j + n - 1;
        uint64_t u_top = (idx + 1 < U.number.size()) ? U.number[idx + 1] : 0;
        uint64_t u_cur = (idx < U.number.size()) ? U.number[idx] : 0;
        uint64_t q_hat = (u_top * BASE + u_cur) / V.number.back();
        if (q_hat >= BASE) q_hat = BASE - 1;
        while (true) {
            uint64_t carry = 0;
            bool tooBig = false;
            for (size_t k = 0; k <= n; ++k) {
                uint64_t vk = (k < n) ? V.number[k] : 0;
                uint64_t prod = q_hat * vk + carry;
                carry = prod / BASE;
                uint64_t uk = (j + k < U.number.size()) ? U.number[j + k] : 0;
                uint64_t diff = uk - (prod % BASE);
                if (diff > uk) {
                    tooBig = true;
                    break;
                }
            }
            if (!tooBig) break;
            --q_hat;
        }
        uint64_t borrow = 0;
        for (size_t k = 0; k <= n; ++k) {
            uint64_t vk = (k < n) ? V.number[k] : 0;
            uint64_t prod = q_hat * vk + borrow;
            borrow = prod / BASE;
            uint64_t diff = U.number[j + k] - (prod % BASE);
            if (diff > U.number[j + k]) {
                diff += BASE;
                ++borrow;
            }
            U.number[j + k] = static_cast<int>(diff);
        }
        q[j] = static_cast<int>(q_hat);
    }
    while (q.size() > 1 && q.back() == 0) q.pop_back();
    BigInteger quotient;
    quotient.number.swap(q);
    quotient.trim();
    return quotient;
}*/

BigInteger BigInteger::operator/(const BigInteger& other) const {
    if (other.isZero()) throw runtime_error("Division by zero");
    if (isZero()) return BigInteger(0);
    if (*this < other) return BigInteger(0);
    if (other == BigInteger(1)) return *this;

    BigInteger dividend = *this;
    BigInteger divisor = other;
    BigInteger quotient;
    BigInteger current;

    for (int i = (int)dividend.number.size() - 1; i >= 0; --i) {
        current.number.insert(current.number.begin(), dividend.number[i]);
        current.trim();

        int q_digit = 0;
        int left = 0, right = BASE - 1;
        while (left <= right) {
            int mid = (left + right) / 2;
            BigInteger prod = divisor * mid;
            if (prod <= current) {
                q_digit = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        quotient.number.insert(quotient.number.begin(), q_digit);
        current = current - divisor * q_digit;
    }
    quotient.trim();
    return quotient;
}

BigInteger BigInteger::operator%(const BigInteger& other) const{
    if (other.isZero()) throw runtime_error("Modulo by zero");
    if (*this < other) return *this;
    BigInteger quotient = *this / other;
    BigInteger remainder = *this - (quotient * other);
    return remainder;
}

BigInteger gcd(const BigInteger& a, const BigInteger& b) {
    BigInteger x = a;
    BigInteger y = b;
    while (!y.isZero()) {
        BigInteger r = x % y;
        x = y;
        y = r;
    }
    return x;
}

BigInteger BigInteger::operator*(int x) const{
    if (x < 0) throw runtime_error("Отрицательное умножение");
    return *this * static_cast<unsigned int>(x);
}
