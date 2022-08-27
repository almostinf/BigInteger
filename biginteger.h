#ifndef BCHECK
#define BCHECK

#include "bitset.h"
#include <string>
#include <exception>

typedef long l;
typedef unsigned long ul;
typedef long long int ll;
typedef unsigned long long ull;

class BigInteger {
private:
    Bitset tape;
    void setup_for_signed(ll);
    void setup_for_unsigned(ull);
    friend BigInteger unsign_shift_left(BigInteger const&, size_t);
    friend BigInteger unsign_shift_right(BigInteger const&, size_t);
public:
    void increase_tape();
    BigInteger();
    BigInteger(int);
    BigInteger(unsigned int);
    BigInteger(l);
    BigInteger(ul);
    BigInteger(ll);
    BigInteger(ull);
    BigInteger(std::string);
    BigInteger(BigInteger const&);
    BigInteger(BigInteger&&);

    BigInteger& operator=(BigInteger const&);
    BigInteger& operator=(BigInteger&&);

    friend bool operator<(BigInteger const&, BigInteger const&);
    friend bool operator>(BigInteger const&, BigInteger const&);
    friend bool operator<=(BigInteger const&, BigInteger const&);
    friend bool operator>=(BigInteger const&, BigInteger const&);
    friend bool operator==(BigInteger const&, BigInteger const&);
    friend bool operator!=(BigInteger const&, BigInteger const&);

    friend BigInteger operator+(BigInteger const&, BigInteger const&);
    friend BigInteger operator-(BigInteger const&, BigInteger const&);
    friend BigInteger operator*(BigInteger const&, BigInteger const&);
    friend BigInteger operator/(BigInteger const&, BigInteger const&);
    friend BigInteger operator%(BigInteger const&, BigInteger const&);
    BigInteger operator++(int);
    BigInteger& operator++();
    BigInteger operator--(int);
    BigInteger& operator--();
    BigInteger& operator+=(BigInteger const&);
    BigInteger& operator-=(BigInteger const&);
    BigInteger& operator*=(BigInteger const&);
    BigInteger& operator/=(BigInteger const&);
    BigInteger& operator%=(BigInteger const&);
    BigInteger& operator+();
    BigInteger operator-();

    friend BigInteger operator^(BigInteger const&, BigInteger const&);
    friend BigInteger operator&(BigInteger const&, BigInteger const&);
    friend BigInteger operator|(BigInteger const&, BigInteger const&);
    BigInteger operator<<(size_t);
    BigInteger operator>>(size_t);
    BigInteger& operator^=(BigInteger const&);
    BigInteger& operator|=(BigInteger const&);
    BigInteger& operator&=(BigInteger const&);
    BigInteger& operator<<=(size_t);
    BigInteger& operator>>=(size_t);
    BigInteger& operator~();

    friend BigInteger negative(BigInteger);
    friend BigInteger positive(BigInteger);
    friend int GetSign(BigInteger);
    friend std::string to_string(BigInteger const&);

    bool empty() const;
};
BigInteger negative(BigInteger);

#endif