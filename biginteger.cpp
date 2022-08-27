#include "biginteger.h"
#include <iostream>

const size_t default_tape_size = 128;

BigInteger unsign_shift_right(BigInteger const& BigInt, size_t shift) {
    BigInteger fake(BigInt);
    auto reverse_Bitset = [](Bitset const& bs) {
        Bitset temp(bs.max_size());
        for (size_t i = 0; i < bs.size(); ++i)
            temp[bs.size() - 1 - i] = bs[i];
        return temp;
    };
    Bitset reverse_tape(std::move(reverse_Bitset(fake.tape)));
    Bitset temp(fake.tape.max_size());
    for (size_t i = 0; i < reverse_tape.size(); ++i)
        if (i + shift < reverse_tape.size())
            temp[i + shift] = static_cast<bool>(reverse_tape[i]);
    Bitset back_to_normal(std::move(reverse_Bitset(temp)));
    fake.tape = std::move(back_to_normal);
    return fake;
}

BigInteger unsign_shift_left(BigInteger const& BigInt, size_t shift) {
    BigInteger fake(BigInt);
    Bitset temp_tape(fake.tape.max_size());
    for (size_t i = 0; i < fake.tape.size(); ++i)
        if ((i + shift) < (fake.tape.size()))
            temp_tape[i + shift] = static_cast<bool>(fake.tape[i]);
    fake.tape = std::move(temp_tape);
    return fake;
}

bool checking_on_correctness(std::string str) {
    if (str.empty()) return false;
    if (str.size() == 1 and !(str[0] >= '0' and str[0] <= '9')) return false;
    for (size_t i = 0; i < str.size(); ++i) {
        if (i == 0 and (str[i] != '-' and str[i] != '+' and !(str[i] >= '0' and str[i] <= '9')))
            return false;
        else if (i != 0 and !(str[i] >= '0' and str[i] <= '9'))
            return false;
    }
    return true;
}

BigInteger::BigInteger(std::string init) : tape(default_tape_size) {
    if (!checking_on_correctness(init)) throw std::invalid_argument("error with"
                                                                          "arguments");
    size_t l = 0;
    size_t n = init.length();
    bool minus = false;
    std::string new_init(init);
    if (init[0] == '-') {
        minus = true;
        new_init.erase(0, 1);
        n--;
    } else if (init[0] == '+') {
        new_init.erase(0, 1);
        n--;
    }
    for (size_t q = 0; q<n; ++q)
        new_init[q] -= '0';
    size_t count_Bitset = 0;
loop:
    tape[count_Bitset++] = new_init[n-1] & 1;
    for (int d = 0, p; l < n; ++l) {
        p = d ? 5 : 0;
        d = new_init[l] & 1;
        new_init[l] >>= 1;
        new_init[l] += p;
        if (new_init[l]) {
            for (size_t q = l + 1; q < n; ++q) {
                p = d ? 5 : 0;
                d = new_init[q] & 1;
                new_init[q] >>= 1;
                new_init[q] += p;
            }
            goto loop;
        }
    }
    if (tape.leftover_to_block() == 0) tape[count_Bitset++] = false;
    size_t temp_left = tape.leftover_to_block();
    tape[temp_left + count_Bitset + 31] = false;
    if (minus) {
        for (size_t i = 0; i < tape.size(); ++i)
            tape[i] = !tape[i];
        short leftover = 1;
        for (size_t i = 0; i < tape.size(); ++i) {
            short digit = tape[i] + leftover;
            if (digit == 2) tape[i] = false;
            else if (digit == 1) {
                tape[i] = true;
                break;
            }
        }
    }
}

void BigInteger::setup_for_signed(ll num) {
    size_t Bitset_count = 0;
    bool sign = num > 0;
    while (num) {
        bool bit = static_cast<bool>(num % 2);
        tape[Bitset_count++] = bit;
        num /= 2;
    }
    if (tape.leftover_to_block() == 0) tape[Bitset_count++] = false;
    size_t temp_left = tape.leftover_to_block();
    for (size_t i = 0; i < temp_left; ++i)
        tape[Bitset_count++] = false;
    if (!sign) {
        for (size_t i = 0; i < tape.size(); ++i)
            tape[i] = !tape[i];
        short leftover = 1;
        for (size_t i = 0; i < tape.size(); ++i) {
            short digit = tape[i] + leftover;
            if (digit == 2) tape[i] = false;
            else if (digit == 1) {
                tape[i] = true;
                break;
            }
        }
    }
}

void BigInteger::setup_for_unsigned(ull num) {
    size_t Bitset_count = 0;
    while (num) {
        bool bit = static_cast<bool>(num % 2);
        tape[Bitset_count++] = bit;
        num /= 2;
    }
    if (tape.leftover_to_block() == 0) tape[Bitset_count++] = false;
    size_t temp_left = tape.leftover_to_block();
    tape[tape.size() + temp_left] = false;
    tape.print();
}

BigInteger::BigInteger(ll num) : tape(default_tape_size) {
    setup_for_signed(num);
}

BigInteger::BigInteger(ull num) : tape(default_tape_size) {
    setup_for_unsigned(num);
}

BigInteger::BigInteger(int num) : tape(default_tape_size) {
    setup_for_signed(num);
}

BigInteger::BigInteger(unsigned int num) : tape(default_tape_size) {
    setup_for_unsigned(num);
}

BigInteger::BigInteger(l num) : tape(default_tape_size) {
    setup_for_signed(num);
}

BigInteger::BigInteger(ul num) : tape(default_tape_size) {
    setup_for_unsigned(num);
}

BigInteger::BigInteger() : tape(default_tape_size) {}

std::string to_string(const BigInteger& BigInt) {
    auto mult_on_2 = [](std::string& num) {
        short leftover = 0;
        int i = 0;
        while (true) {
            if (i == num.size()) break;
            short digit = num[i] - '0';
            short new_digit = (digit * 2) + leftover;
            if (new_digit < 10) {
                num[i] = '0' + new_digit;
                leftover = 0;
            }
            else {
                num[i] = '0' + (new_digit - 10);
                if (i + 1 == num.size()) {
                    num += '1';
                    break;
                }
                leftover = 1;
            }
            i++;
        }
    };
    auto add_1 = [](std::string const& left, std::string const& right) {
        std::string res_add;
        size_t max_size = left.size() > right.size() ? left.size() : right.size();
        short leftover = 0;
        for (size_t i = 0; i < max_size; ++i) {
            if (i < left.size() and i < right.size()) {
                short digit = (left[i] - '0') + (right[i] - '0') + leftover;
                if (digit < 10) {
                    res_add += '0' + digit;
                    leftover = 0;
                } else {
                    res_add += '0' + (digit - 10);
                    leftover = 1;
                }
            } else if (i < left.size()) {
                short digit = (left[i] - '0') + leftover;
                if (digit < 10) {
                    res_add += '0' + digit;
                    leftover = 0;
                } else {
                    res_add += '0' + (digit - 10);
                    leftover = 1;
                }
            } else if (i < right.size()) {
                short digit = (right[i] - '0') + leftover;
                if (digit < 10) {
                    res_add += '0' + digit;
                    leftover = 0;
                } else {
                    res_add += '0' + (digit - 10);
                    leftover = 1;
                }
            }
            if (i == max_size - 1 and leftover) res_add += '1';
        }
        return res_add;
    };
    std::string res = "0";
    BigInteger BI(BigInt);
    bool minus = false;
    //check on negative
    if (BI.tape[BI.tape.size() - 1]) {
        minus = true;
        for (size_t i = 0; i < BI.tape.size(); ++i)
            BI.tape[i] = !BI.tape[i];
        short leftover = 1;
        for (size_t i = 0; i < BI.tape.size(); ++i) {
            short digit = BI.tape[i] + leftover;
            if (digit == 2) BI.tape[i] = false;
            else if (digit == 1) {
                BI.tape[i] = true;
                break;
            }
        }
    }
    size_t deg = 0;
    for (size_t i = 0; i < BI.tape.size() - 1; ++i) {
        if (BI.tape[i]) {
            size_t temp_deg = deg;
            std::string temp = "1";
            while (temp_deg--)
                mult_on_2(temp);
            res = add_1(res, temp);
        }
        deg++;
    }
    if (minus) res += '-';
    for (int i = 0; i < res.size() / 2; i++)
        std::swap(res[i], res[res.size() - i - 1]);
    return res;
}

BigInteger::BigInteger(BigInteger const& BigInt) : tape(BigInt.tape) {}

BigInteger::BigInteger(BigInteger&& BigInt) : tape(std::move(BigInt.tape)) {}

BigInteger& BigInteger::operator=(const BigInteger& BigInt) {
    if (this != &BigInt)
        tape = BigInt.tape;
    return *this;
}

BigInteger& BigInteger::operator=(BigInteger&& BigInt) {
    tape = std::move(BigInt.tape);
    return *this;
}


bool operator<(const BigInteger& left, const BigInteger& right) {
    if (left.tape.size() > right.tape.size()) return false;
    else {
        BigInteger temp_left(left);
        BigInteger temp_right(right);
        bool left_sign = !left.tape[left.tape.size() - 1];
        bool right_sign = !right.tape[right.tape.size() - 1];
        size_t max_size = temp_right.tape.size() > temp_left.tape.size() ? temp_right.tape.size()
                : temp_left.tape.size();
        if (left_sign == right_sign) {
            if (!left_sign and !right_sign) {
                temp_left = positive(left);
                temp_right = positive(right);
                max_size = temp_right.tape.size() > temp_left.tape.size()
                        ? temp_right.tape.size() : temp_left.tape.size();
                for (int i = max_size - 1; i >= 0; --i) {
                    if (!temp_left.tape[i] and temp_right.tape[i]) return false;
                    else if (temp_left.tape[i] and !temp_right.tape[i]) return true;
                }
            } else {
                for (int i = max_size - 1; i >= 0; --i) {
                    if (!temp_left.tape[i] and temp_right.tape[i]) return true;
                    else if (temp_left.tape[i] and !temp_right.tape[i]) return false;
                }
            }
        } else {
            if (left_sign and !right_sign) return false;
            else return true;
        }
    }
}

bool operator==(const BigInteger& left, const BigInteger& right) {
    if (left.tape == right.tape) return true;
    return false;
}

bool operator<=(const BigInteger& left, const BigInteger& right) {
    return left == right or left < right;
}

bool operator>(const BigInteger& left, const BigInteger& right) {
    return right < left;
}

bool operator>=(const BigInteger& left, const BigInteger& right) {
    return right == left or left > right;
}

BigInteger& BigInteger::operator~() {
    for (size_t i = 0; i < tape.size(); ++i)
        tape[i] = !tape[i];
    return *this;
}

BigInteger operator&(BigInteger const& left, BigInteger const& right) {
    BigInteger res;
    size_t max_size = left.tape.size() > right.tape.size() ? left.tape.size() :
            right.tape.size();
    for (size_t i = 0; i < max_size; ++i) {
        if (i < left.tape.size() and i < right.tape.size())
            res.tape[i] = (left.tape[i] and right.tape[i]);
        else if (i < left.tape.size())
            res.tape[i] = false;
        else if (i < right.tape.size())
            res.tape[i] = false;
    }
    return res;
}

BigInteger operator|(BigInteger const& left, BigInteger const& right) {
    BigInteger res;
    size_t max_size = left.tape.size() > right.tape.size() ? left.tape.size() :
                      right.tape.max_size();
    for (size_t i = 0; i < max_size; ++i) {
        if (i < left.tape.size() and i < right.tape.size()) {
            res.tape[i] = (left.tape[i] or right.tape[i]);
        } else if (i < left.tape.size()) {
            res.tape[i] = left.tape[i];
        } else if (i < right.tape.size()) {
            res.tape[i] = right.tape[i];
        }
    }
    return res;
}

BigInteger operator^(BigInteger const& left, BigInteger const& right) {
    BigInteger res;
    size_t max_size = left.tape.size() > right.tape.size() ? left.tape.size() :
                      right.tape.size();
    for (size_t i = 0; i < max_size; ++i) {
        if (i < left.tape.size() and i < right.tape.size()) {
            res.tape[i] = !((left.tape[i] and right.tape[i]) or (!left.tape[i] and
                                                                 !right.tape[i]));
        } else if (i < left.tape.size()) {
            res.tape[i] = left.tape[i];
        } else if (i < right.tape.size()) {
            res.tape[i] = right.tape[i];
        }
    }
    return res;
}

BigInteger BigInteger::operator<<(size_t shift) {
    BigInteger fake(*this);
    Bitset temp_tape(fake.tape.max_size());
    for (size_t i = 0; i < fake.tape.size(); ++i)
        if ((i + shift) < (fake.tape.size()))
            temp_tape[i + shift] = static_cast<bool>(fake.tape[i]);
    fake.tape = std::move(temp_tape);
    return fake;
}

BigInteger BigInteger::operator>>(size_t shift) {
    BigInteger fake(*this);
    auto reverse_Bitset = [](Bitset const& bs) {
        Bitset temp(bs.max_size());
        for (size_t i = 0; i < bs.size(); ++i)
            temp[bs.size() - 1 - i] = bs[i];
        return temp;
    };
    Bitset reverse_tape(std::move(reverse_Bitset(fake.tape)));
    Bitset temp(fake.tape.max_size());
    if (GetSign(fake))
        for (size_t i = 0; i < shift; ++i)
            temp[i] = true;
    for (size_t i = 0; i < reverse_tape.size(); ++i)
        if (i + shift < reverse_tape.size())
            temp[i + shift] = static_cast<bool>(reverse_tape[i]);
    Bitset back_to_normal(std::move(reverse_Bitset(temp)));
    fake.tape = std::move(back_to_normal);
    return fake;
}

bool BigInteger::empty() const {
    for (size_t i = 0; i < tape.size(); ++i)
        if (tape[i]) return false;
    return true;
}

BigInteger operator+(BigInteger const& left, BigInteger const& right) {
    BigInteger Res, Carry;
    BigInteger newLeft(left);
    BigInteger newRight(right);

    if (left.tape.size() > right.tape.size()) {
        size_t dif = left.tape.size() - right.tape.size();
        size_t j = right.tape.size();
        if (GetSign(right))
            for (size_t i = 0; i < dif; ++i)
                newRight.tape[j++] = true;
        else
            for (size_t i = 0; i < dif; ++i)
                newRight.tape[j++] = false;
    } else if (right.tape.size() > left.tape.size()) {
        size_t dif = right.tape.size() - left.tape.size();
        size_t j = left.tape.size();
        if (GetSign(left))
            for (size_t i = 0; i < dif; ++i)
                newLeft.tape[j++] = true;
        else
            for (size_t i = 0; i < dif; ++i)
                newLeft.tape[j++] = false;
    }
    Res = newLeft ^ newRight;
    Carry = (newLeft & newRight);
    Carry = unsign_shift_left(Carry, 1);
    while (!Carry.empty()) {
        BigInteger Tmp = Res;
        Res = Res ^ Carry;
        Carry = unsign_shift_left((Tmp & Carry), 1);
    }
    return Res;
}

BigInteger negative(BigInteger BigInt) {
    for (size_t i = 0; i < BigInt.tape.size(); ++i)
        BigInt.tape[i] = !BigInt.tape[i];
    short leftover = 1;
    for (size_t i = 0; i < BigInt.tape.size(); ++i) {
        short digit = BigInt.tape[i] + leftover;
        if (digit == 2) BigInt.tape[i] = false;
        else if (digit == 1) {
            BigInt.tape[i] = true;
            break;
        }
    }
    return BigInt;
}

BigInteger operator-(BigInteger const& left, BigInteger const& right) {
    return left + negative(right);
}

int GetSign(BigInteger BigInt) {
    return BigInt.tape[BigInt.tape.size() - 1];
}

BigInteger positive(BigInteger BigInt) {
    return (GetSign(BigInt) & 1) ? negative(BigInt) : BigInt;
}


BigInteger operator*(BigInteger const& left, BigInteger const& right) {
    bool isNegative = false;
    if (GetSign(left) ^ GetSign(right))
        isNegative = true;
    if (left.empty() or right.empty()) return {};
    BigInteger pLeft(positive(left));
    BigInteger pRight(positive(right));
    BigInteger Res;
    Res.tape[(pRight.tape.cur_block() + pLeft.tape.cur_block()) * 32 - 1] = false;
    pLeft.tape[(pRight.tape.cur_block() + pLeft.tape.cur_block()) * 32 - 1] = false;
    while (!((pRight | 0).empty())) {
        if (!((pRight & 1).empty()))
            Res = Res + pLeft;
        pLeft = unsign_shift_left(pLeft, 1);
        pRight = unsign_shift_right(pRight, 1);
    }
    return isNegative ? negative(Res) : Res;
}

BigInteger operator/(BigInteger const& Quotient, BigInteger const& Divisor) {
    bool isNegative = false;
    if (GetSign(Quotient) ^ GetSign(Divisor))
        isNegative = true;
    BigInteger Dividend(positive(Quotient));
    BigInteger pDivisor(positive(Divisor));
    pDivisor.tape[Dividend.tape.size() - 1] = false;
    BigInteger Remainder;
    Remainder.tape[Dividend.tape.size() - 1] = false;
    size_t count = Dividend.tape.find_one_abs();
    size_t last_bit = count;
    count++;
    while (count) {
        bool bit = Dividend.tape[last_bit];
        Dividend = unsign_shift_left(Dividend, 1);
        Remainder = unsign_shift_left(Remainder, 1);
        if (bit)
            Remainder = Remainder | bit;
        BigInteger Substr = Remainder - pDivisor;
        if (!GetSign(Substr)) {
            Dividend = Dividend | 1;
            Remainder = Substr;
        }
        count--;
    }
    for (size_t i = last_bit; i < Dividend.tape.size(); ++i)
        Dividend.tape[i] = false;
    return isNegative ? negative(Dividend) : Dividend;
}

BigInteger operator%(BigInteger const& Quotient, BigInteger const& Divisor) {
    bool isNegative = GetSign(Quotient);
    BigInteger Dividend(positive(Quotient));
    BigInteger pDivisor(positive(Divisor));
    pDivisor.tape[Dividend.tape.size() - 1] = false;
    BigInteger Remainder;
    Remainder.tape[Dividend.tape.size() - 1] = false;
    size_t count = Dividend.tape.find_one_abs();
    size_t last_bit = count;
    count++;
    while (count) {
        bool bit = Dividend.tape[last_bit];
        Dividend = unsign_shift_left(Dividend, 1);
        Remainder = unsign_shift_left(Remainder, 1);
        if (bit)
            Remainder = Remainder | bit;
        BigInteger Substr = Remainder - pDivisor;
        if (!GetSign(Substr)) {
            Dividend = Dividend | 1;
            Remainder = Substr;
        }
        count--;
    }
    return isNegative ? negative(Remainder) : Remainder;
}

BigInteger& BigInteger::operator++() {
    *this = *this + 1;
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger temp(*this);
    *this = *this + 1;
    return temp;
}

BigInteger& BigInteger::operator--() {
    *this = *this - 1;
    return *this;
}

BigInteger BigInteger::operator--(int) {
    BigInteger temp(*this);
    *this = *this - 1;
    return temp;
}

BigInteger& BigInteger::operator+=(const BigInteger& BigInt) {
    *this = *this + BigInt;
    return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& BigInt) {
    *this = *this - BigInt;
    return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& BigInt) {
    *this = *this * BigInt;
    return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& BigInt) {
    *this = *this / BigInt;
    return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& BigInt) {
    *this = *this % BigInt;
    return *this;
}

bool operator!=(const BigInteger& left, const BigInteger& right) {
    return !(left == right);
}

BigInteger& BigInteger::operator+() {
    return *this;
}

BigInteger BigInteger::operator-() {
    return negative(*this);
}

BigInteger& BigInteger::operator^=(const BigInteger& BigInt) {
    *this = *this ^ BigInt;
    return *this;
}

BigInteger& BigInteger::operator&=(const BigInteger& BigInt) {
    *this = *this & BigInt;
    return *this;
}

BigInteger& BigInteger::operator|=(const BigInteger& BigInt) {
    *this = *this | BigInt;
    return *this;
}

BigInteger& BigInteger::operator<<=(size_t num) {
    *this = *this << num;
    return *this;
}

BigInteger& BigInteger::operator>>=(size_t num) {
    *this = *this >> num;
    return *this;
}