#ifndef BIGFRACTION_H
#define BIGFRACTION_H

#include "biginteger.h"
#include <iostream>

class BigFraction {
public:
    BigFraction();
    BigFraction(const BigInteger& num, const BigInteger& den);
    explicit BigFraction(const BigInteger& num);
    explicit BigFraction(int num);

    bool isZero();
    BigInteger getIntegerPart();
    BigInteger getRemainder();

    BigFraction operator-();

    BigFraction operator+(const BigFraction& other) const;
    BigFraction operator-(const BigFraction& other) const;
    BigFraction operator*(const BigFraction& other) const;
    BigFraction operator/(const BigFraction& other) const;

    BigFraction& operator+=(const BigFraction& other);
    BigFraction& operator-=(const BigFraction& other);
    BigFraction& operator*=(const BigFraction& other);
    BigFraction& operator/=(const BigFraction& other);

    BigFraction operator+(int num) const;
    BigFraction operator-(int num) const;
    BigFraction operator*(int num) const;
    BigFraction operator/(int num) const;

    friend BigFraction operator+(int num, const BigFraction& fr);
    friend BigFraction operator-(int num, const BigFraction& fr);
    friend BigFraction operator*(int num, const BigFraction& fr);
    friend BigFraction operator/(int num, const BigFraction& fr);

    bool operator==(const BigFraction& other)const;
    bool operator!=(const BigFraction& other)const;
    bool operator<(const BigFraction& other)const;
    bool operator<=(const BigFraction& other)const;
    bool operator>(const BigFraction& other)const;
    bool operator>=(const BigFraction& other) const;

    const BigInteger& GetNumerator() const;
    const BigInteger& GetDenominator() const;


private:
    BigInteger num;
    BigInteger den;
    void normalize();
    static void checkDenZero(BigInteger& d);
};

#endif
