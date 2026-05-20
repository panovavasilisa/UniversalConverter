#include "bigfraction.h"
#include "biginteger.h"
#include <stdexcept>

void BigFraction::checkDenZero(BigInteger& d) {
    if (d.isZero()) throw std::logic_error("знаменатель равен 0");
}

BigFraction::BigFraction() : num(0), den(1) {}

BigFraction::BigFraction(const BigInteger& n, const BigInteger& d) : num(n), den(d) {
    checkDenZero(den);
    normalize();
}

BigFraction::BigFraction(const BigInteger& n) : num(n), den(1) {}

BigFraction::BigFraction(int n) : num(n), den(1) {}

bool BigFraction::isZero() {
    return num.isZero();
}

BigInteger BigFraction::getIntegerPart() {
    return num / den;
}

BigInteger BigFraction::getRemainder(){
    return num % den;
}

void BigFraction::normalize() {
    if (den.isZero()) return;
    if (num.isZero()) {
        den = 1;
        return;
    }
    BigInteger g = gcd(num, den);
    if (!g.isZero() && g != 1) {
        num = num / g;
        den = den / g;
    }
}

BigFraction BigFraction::operator-() {
    return BigFraction(num * -1, den);
}

BigFraction BigFraction::operator+(const BigFraction& other) const {
    BigInteger newNum = num * other.den + den * other.num;
    BigInteger newDen = den * other.den;
    return BigFraction(newNum, newDen);
}

BigFraction BigFraction::operator-(const BigFraction& other) const{
    BigInteger newNum = num * other.den - other.num * den;
    if (newNum < 0) newNum = BigInteger(0);
    BigInteger newDen = den * other.den;
    return BigFraction(newNum, newDen);
}

BigFraction BigFraction::operator*(const BigFraction& other) const{
    BigInteger newNum = num * other.num;
    BigInteger newDen = den * other.den;
    return BigFraction(newNum, newDen);
}

BigFraction BigFraction::operator/(const BigFraction& other) const{
    if (other.num.isZero()) throw std::logic_error("деление на 0");
    BigInteger newNum = num * other.den;
    BigInteger newDen = den * other.num;
    return BigFraction(newNum, newDen);
}

BigFraction& BigFraction::operator+=(const BigFraction& other) {
    *this = *this + other;
    return *this;
}

BigFraction& BigFraction::operator-=(const BigFraction& other) {
    *this = *this - other;
    return *this;
}

BigFraction& BigFraction::operator*=(const BigFraction& other) {
    *this = *this * other;
    return *this;
}

BigFraction& BigFraction::operator/=(const BigFraction& other) {
    *this = *this / other;
    return *this;
}

BigFraction BigFraction::operator+(int num) const{
    return *this + BigFraction(num);
}

BigFraction BigFraction::operator-(int num) const{
    return *this - BigFraction(num);
}

BigFraction BigFraction::operator*(int num) const{
    return *this * BigFraction(num);
}

BigFraction BigFraction::operator/(int num) const{
    if (num == 0) throw std::logic_error("деление на 0");
    return *this / BigFraction(num);
}

BigFraction operator+(int num, const BigFraction& fr) {
    return (fr + num);
}

BigFraction operator-(int num, const BigFraction& fr) {
    return BigFraction(num) - fr;
}

BigFraction operator*(int num, const BigFraction& fr) {
    return fr * num;
}

BigFraction operator/(int num, const BigFraction& fr) {
    if (fr.num.isZero()) throw std::logic_error("деление на 0");
    return BigFraction(num) / fr;
}

bool BigFraction::operator==(const BigFraction& other) const {
    return num * other.den == other.num * den;
}

bool BigFraction::operator!=(const BigFraction& other) const {
    return !(*this == other);
}

bool BigFraction::operator<(const BigFraction& other) const {
    return num * other.den < other.num * den;
}

bool BigFraction::operator<=(const BigFraction& other) const {
    return !(*this > other);
}

bool BigFraction::operator>(const BigFraction& other) const {
    return other < *this;
}

bool BigFraction::operator>=(const BigFraction& other) const {
    return !(*this < other);
}



const BigInteger& BigFraction::GetNumerator() const {
    return num;
}

const BigInteger& BigFraction::GetDenominator() const {
    return den;
}

