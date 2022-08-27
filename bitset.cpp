#include "bitset.h"
#include <cstddef>
#include <iostream>

const size_t UINT_SIZE = sizeof(uint) * 8;

Bitset::~Bitset() { delete[] _arr; }

Bitset::Bitset(size_t max_size)
        : _max_size(max_size), _current_size(0), _current_blocks(0), _leftover_to_block(0) {
    if (max_size % UINT_SIZE == 0) // remove checking on max_size / BYTE_SIZE == 0
        _blocks_count = max_size / UINT_SIZE;
    else
        _blocks_count = max_size / UINT_SIZE + 1;
    _arr = new uint[_blocks_count]{0};
}

Bitset::Bitset(Bitset&& bs) : _arr(bs._arr), _max_size(bs._max_size),
        _current_size(bs._current_size), _blocks_count(bs._blocks_count),
        _current_blocks(bs._current_blocks), _leftover_to_block(0) {
    bs._arr = nullptr;
}

bool Bitset::operator[](size_t pos) const {
    size_t need_byte = pos / UINT_SIZE;
    size_t new_pos = pos % UINT_SIZE;
    return static_cast<bool>(_arr[need_byte] & (1 << new_pos));
}

Bitset::Bit Bitset::operator[](size_t pos) {
    if (pos >= _max_size)
        realloc();
    if (pos > _current_size)
        _current_size = pos;
    size_t need_byte = pos / UINT_SIZE;
    if (need_byte > _current_blocks)
        _current_blocks = need_byte;
    size_t new_pos = pos % UINT_SIZE;
    Bitset::Bit bit(&_arr[need_byte], new_pos);
    return bit;
}

Bitset &Bitset::operator=(const Bitset &bs) {
    // copy and swap idiom
    auto swap = [&](Bitset &mbs) -> void {
        size_t temp = _max_size;
        _max_size = mbs._max_size;
        mbs._max_size = temp;
        temp = _current_blocks;
        _current_blocks = mbs._current_blocks;
        mbs._current_blocks = temp;
        temp = _current_size;
        _current_size = mbs._current_size;
        mbs._current_size = temp;
        temp = _blocks_count;
        _blocks_count = mbs._blocks_count;
        uint *p_temp = _arr;
        _arr = mbs._arr;
        mbs._arr = p_temp;
        temp = _leftover_to_block;
        _leftover_to_block = mbs._leftover_to_block;
        mbs._leftover_to_block = temp;
    };
    if (&bs != this) {
        Bitset temp(bs);
        swap(temp);
    }
    return *this;
}

Bitset& Bitset::operator=(Bitset&& bs) {
    _arr = bs._arr;
    bs._arr = nullptr;
    _blocks_count = bs._blocks_count;
    _max_size = bs._max_size;
    _current_blocks = bs._current_blocks;
    _current_size = bs._current_size;
    _leftover_to_block = bs._leftover_to_block;
    return *this;
}

Bitset::Bitset(const Bitset &bs) {
    _max_size = bs._max_size;
    _blocks_count = bs._blocks_count;
    _current_blocks = bs._current_blocks;
    _current_size = bs._current_size;
    _leftover_to_block = bs._leftover_to_block;
    // delete[] _arr;
    _arr = new uint[_blocks_count];
    for (size_t i = 0; i < _blocks_count; ++i)
        _arr[i] = bs._arr[i];
}

size_t Bitset::max_size() const { return _max_size; }

size_t Bitset::size() const { return _current_size + 1; }

size_t Bitset::cur_block() const { return _current_blocks + 1; }

size_t Bitset::max_blocks() const { return _blocks_count; }

size_t Bitset::leftover_to_block() const {
    size_t cur_pos = _current_size % 32;
    return 31 - cur_pos;
}

void Bitset::realloc() {
    _max_size *= 4;
    _blocks_count = _max_size / UINT_SIZE + 1;
    uint *temp_arr = _arr;
    _arr = new uint[_blocks_count]{0};
    for (size_t i = 0; i <= _current_blocks; ++i)
        _arr[i] = temp_arr[i];
    delete[] temp_arr;
}

void Bitset::print() const {
    for (size_t i = 0; i <= _current_blocks; ++i) {
        for (size_t j = 0; j < UINT_SIZE; ++j)
            std::cout << static_cast<bool>(_arr[i] & (1 << j));
        std::cout << ' ';
    }
    std::cout << '\n';
}

void Bitset::clear() {
    for (size_t i = 0; i < _current_blocks; ++i)
        _arr[i] = 0;
    _current_blocks = 0;
    _current_size = 0;
}

Bitset Bitset::Bitset_cur_size() const {
    Bitset b(_current_size + 1);
    for (size_t i = 0; i <= _current_blocks; ++i)
        b._arr[i] = _arr[i];
    return b;
}

uint *Bitset::get_arr() const { return _arr; }

bool Bitset::operator==(const Bitset& bs) const {
    bool this_sign = (*this)[_current_size];
    bool bs_sign = bs[bs._current_size];
    if (this_sign != bs_sign) return false;
    size_t max_size = _current_size > bs._current_size ? _current_size : bs._current_size;
    for (size_t i = 0; i < max_size; ++i) {
        if (i <= _current_size and i <= bs._current_size and (*this)[i] != bs[i])
            return false;
        else if (i <= _current_size and i > bs._current_size and (this_sign != (*this)[i]))
            return false;
        else if (i <= bs._current_size and i > _current_size and (bs_sign != bs[i]))
            return false;
    }
    return true;
}

size_t Bitset::find_one_abs() const {
    for (int i = _current_size; i >= 0; --i)
        if ((*this)[i]) return i;
}