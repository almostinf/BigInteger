#ifndef CHECK
#define CHECK

// libraries
#include <cstddef>

typedef unsigned int uint;

class Bitset {
private:
    size_t _max_size;
    uint *_arr;
    size_t _blocks_count;
    size_t _current_size;
    size_t _current_blocks;
    size_t _leftover_to_block;
    class Bit {
    private:
        uint *_byte;
        size_t _bit_idx;

    public:
        Bit(uint *byte, size_t bit_idx) : _byte(byte), _bit_idx(bit_idx){};
        Bit &operator=(bool bit) {
            if (bit)
                *_byte |= 1 << _bit_idx;
            else
                *_byte &= ~(1 << _bit_idx);
            return *this;
        }
        operator bool() const {
            return static_cast<bool>(*_byte & (1 << _bit_idx));
        }
    };
    void realloc();
public:
    Bitset(size_t);
    Bitset(Bitset const &);
    Bitset(Bitset&&);
    ~Bitset();

    bool operator[](size_t) const;
    Bit operator[](size_t);
    Bitset &operator=(Bitset const &);
    Bitset& operator=(Bitset&&);
    bool operator==(Bitset const&) const;
    size_t max_size() const;
    size_t size() const;
    size_t cur_block() const;
    size_t max_blocks() const;
    size_t leftover_to_block() const;
    size_t find_one_abs() const;
    void print() const;
    void clear();

    Bitset Bitset_cur_size() const;
    uint *get_arr() const;
};

#endif // !CHECK