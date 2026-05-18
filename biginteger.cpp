#include "biginteger.h"
#include <string>
#include <vector>
#include <stdexcept>
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

string BigInteger::toString() const {
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

bool BigInteger::operator==(const BigInteger& other) const {
    if (number.size() != other.number.size()) return false;
    for (size_t i = 0; i < number.size(); ++i) {
        if (number[i] != other.number[i]) return false;
    }
    return true;
}

bool BigInteger::operator!=(const BigInteger& other) const {
    return !(*this == other);
}

bool BigInteger::operator<(const BigInteger& other) const {
    if (number.size() != other.number.size())
        return number.size() < other.number.size();
    for (int i = (int)number.size() - 1; i >= 0; --i) {
        if (number[i] != other.number[i])
            return number[i] < other.number[i];
    }
    return false;
}

bool BigInteger::operator<=(const BigInteger& other) const {
    return (*this < other) || (*this == other);
}

bool BigInteger::operator>(const BigInteger& other) const {
    return !(*this <= other);
}

bool BigInteger::operator>=(const BigInteger& other) const {
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

BigInteger BigInteger::operator+(const BigInteger& other) const {
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

BigInteger BigInteger::operator-(const BigInteger& other) const {
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

BigInteger BigInteger::operator*(unsigned int x) const {
    BigInteger result = *this;
    result *= x;
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

BigInteger BigInteger::operator/(unsigned int x) const {
    BigInteger result = *this;
    result /= x;
    return result;
}

unsigned int BigInteger::operator%(unsigned int x) const {
    if (x == 0) throw runtime_error("деление на 0");
    uint64_t rem = 0;
    for (int i = (int)number.size() - 1; i >= 0; --i) {
        rem = (rem * BASE + number[i]) % x;
    }
    return static_cast<unsigned int>(rem);
}

/*void BigInteger::karatsuba(int* a, int* b, long long* c, int n) {
    if (n <= 64) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                c[i + j] += (long long)a[i] * b[j];
            }
        }
        long long carry = 0;
        for (int i = 0; i < 2 * n; ++i) {
            c[i] += carry;
            carry = c[i] / BASE;
            c[i] %= BASE;
        }
        return;
    }
    int k = n / 2;
    vector<int> l(k), r(k);
    for (int i = 0; i < k; ++i) {
        l[i] = a[i] + a[k + i];
        r[i] = b[i] + b[k + i];
    }
    int carry = 0;
    for (int i = 0; i < k; ++i) {
        l[i] += carry;
        carry = l[i] / BASE;
        l[i] %= BASE;
    }
    carry = 0;
    for (int i = 0; i < k; ++i) {
        r[i] += carry;
        carry = r[i] / BASE;
        r[i] %= BASE;
    }
    vector<long long> ac(2 * k, 0);
    vector<long long> bd(2 * k, 0);
    vector<long long> t(2 * k, 0);
    karatsuba(a + k, b + k, ac.data(), k);
    karatsuba(a, b, bd.data(), k);
    karatsuba(l.data(), r.data(), t.data(), k);
    for (int i = 0; i < 2 * k; ++i) {
        c[i] = bd[i];
        c[2 * k + i] = ac[i];
    }
    for (int i = 0; i < 2 * k; ++i) {
        long long val = t[i] - ac[i] - bd[i];
        c[k + i] += val;
    }
    long long carry2 = 0;
    for (int i = 0; i < 4 * k; ++i) {
        c[i] += carry2;
        carry2 = c[i] / BASE;
        c[i] %= BASE;
    }
}*/


BigInteger& BigInteger::operator*=(BigInteger& other) {
    if (isZero() || other.isZero()) {
        *this = BigInteger(0);
        return *this;
    }
    size_t max_len = max(number.size(), other.number.size());
    size_t n = 1;
    while (n < max_len) n <<= 1;
    int* a = new int[n]();
    int* b = new int[n]();
    for (size_t i = 0; i < number.size(); ++i) a[i] = number[i];
    for (size_t i = 0; i < other.number.size(); ++i) b[i] = other.number[i];
    long long* c = new long long[4 * n]();
    karatsuba(a, b, c, (int)n);
    vector<int> res;
    uint64_t carry = 0;
    for (int i = 0; i < 4 * n; ++i) {
        uint64_t val = (uint64_t)c[i] + carry;
        res.push_back((int)(val % BASE));
        carry = val / BASE;
    }
    while (carry) {
        res.push_back((int)(carry % BASE));
        carry /= BASE;
    }
    delete[] a;
    delete[] b;
    delete[] c;
    number.swap(res);
    trim();
    return *this;
}

BigInteger& BigInteger::operator*=(BigInteger& other) {
    if (isZero() || other.isZero()) {
        *this = BigInteger(0);
        return *this;
    }
    vector<long long> res_digits = karatsuba(number, other.number);
    number.clear();
    number.reserve(res_digits.size());
    for (long long v : res_digits) number.push_back(static_cast<int>(v));
    trim();
    return *this;
}
BigInteger BigInteger::operator*(BigInteger& other) {
    BigInteger result = *this;
    result *= other;
    return result;
}

BigInteger BigInteger::operator/(BigInteger& other) {
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
}

BigInteger BigInteger::operator%(BigInteger& other) {
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



BigInteger BigInteger::operator*(int x) const {
    if (x < 0) throw std::runtime_error("Отрицательное умножение");
    return *this * static_cast<unsigned int>(x);
}
