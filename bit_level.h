#ifndef BIT_LEVEL_H
#define BIT_LEVEL_H

#include <fstream>
#include <stdio.h>
#include <exception>

typedef bool bit;


class EOFexception : std::exception
{
public:
    const char* what() const throw() {return "Reached end of file";}
};


namespace bitstream {

inline uint8_t bit2byte(int n)
{
    switch (n) {
    case 0: return 1;
    case 1: return 2;
    case 2: return 4;
    case 3: return 8;
    case 4: return 16;
    case 5: return 32;
    case 6: return 64;
    case 7: return 128;
    default: return 0;
    }
}


class writer
{
public:
    writer(const std::string & outfile) {
        f.open(outfile);
        if(!f.is_open()) throw "Could not open output file";
    };

    void close()
    {
        f.close();
    }

    void push(const bit b)
    {
        bitsleft--;
        if(b)
        {
            uint8_t mask = bit2byte(bitsleft);
            curr_buffer |= mask; // flipping bit
        }
        if(!bitsleft)
        {
            f << (unsigned char) curr_buffer;
            curr_buffer = 0;
            bitsleft = wordsize;
        }
    }
    const int wordsize = 8;
protected:
    std::ofstream f;
    int bitsleft = wordsize;
    uint8_t curr_buffer = 0;
};


class reader
{
public:
    reader(const std::string & infile) {
        f.open(infile);
        if(!f.is_open()) throw "Could not open output file";
    };

    void close()
    {
        f.close();
    }

    bit pull()
    {
        if(!bitsleft)
        {
            char c;
            if(!f.get(c)) throw EOFexception();
            curr_buffer = (uint8_t) c;
            bitsleft = wordsize;
        }
        bitsleft--;
        uint8_t mask = bit2byte(bitsleft);
        bit b = (mask & curr_buffer) != 0;
        return b;
    }
    const int wordsize = 8;
protected:
    std::ifstream f;
    int bitsleft = 0;
    uint8_t curr_buffer = 0;
};

}
#endif // BIT_LEVEL_H
