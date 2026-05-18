// biginteger.h
#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include <vector>
#include <string>
using namespace std;

class BigInteger {
public:
    BigInteger();
    BigInteger(int num);
    BigInteger(string num);

    bool isZero() const;
    string toString() const;
    void trim();

    bool operator==(const BigInteger& other) const;
    bool operator!=(const BigInteger& other) const;
    bool operator<(const BigInteger& other) const;
    bool operator<=(const BigInteger& other) const;
    bool operator>(const BigInteger& other) const;
    bool operator>=(const BigInteger& other) const;

    BigInteger operator+(const BigInteger& other) const;
    BigInteger& operator+=(const BigInteger& other);
    BigInteger operator-(const BigInteger& other) const;
    BigInteger& operator-=(const BigInteger& other);

    BigInteger operator*(unsigned int x) const;
    BigInteger& operator*=(unsigned int x);
    BigInteger operator*(BigInteger& other);
    BigInteger& operator*=(BigInteger& other);

    BigInteger operator/(unsigned int x) const;
    BigInteger& operator/=(unsigned int x);
    BigInteger operator/(BigInteger& other);
    unsigned int operator%(unsigned int x) const;
    BigInteger operator%(BigInteger& other);

    BigInteger operator*(int x) const;

    void karatsuba(int* a, int* b, long long* c, int n);

private:
    static const int BASE = 1000000000;
    vector<int> number;
};

BigInteger gcd(const BigInteger& a, const BigInteger& b);

#endif
