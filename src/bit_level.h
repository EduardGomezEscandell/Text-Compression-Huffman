#ifndef BIT_LEVEL_H
#define BIT_LEVEL_H

#define WORDSIZE 8

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <exception>

#define OPEN(f, filename) do{                                                       \
    f.open(filename);                                                               \
    if(!f.is_open())                                                                \
    {                                                                               \
        std::cerr << "File " << filename <<" could not be accessed."<<std::endl;    \
        throw "Could not open input file";                                          \
    }                                                                               \
    } while(false)

typedef bool bit;


namespace  bitstream {

class writer
{
public:
    writer(const std::string & outfile) {
        OPEN(f,outfile);
    };

    ~writer()
    {
        if(bitcount != 0)
        std::cerr<<"[WARNING] There were unprinted bits in the buffer. Use bitsream::writer::eof() to print them before destroying the writer." <<std::endl;
    }

    void close()
    {
        f.close();
    }

    void push(const bit b)
    {
        buffer = buffer << 1;
        buffer |= b;
        bitcount++;
        if(bitcount == WORDSIZE) flush();
    }

    void push(const unsigned char ch)
    {
        // TODO Replace with something more efficient
        uint8_t mask = 1 << (WORDSIZE-1);
        while(mask)
        {
            bit b = ch & mask;
            push(b);
            mask = mask >> 1;
        }

    }

    void eof()
    {
        // Pushing zeros until it flushes
        while (bitcount > 0) {
            push(false);
        }
    }

protected:
    inline void flush()
    {
        f << buffer;
        buffer = 0;
        bitcount = 0;
    }
    std::ofstream f;
    short bitcount = 0;
    uint8_t buffer = 0;
};


class reader
{
public:
    reader(const std::string & infile) {
        OPEN(f,infile);
    };

    void close()
    {
        f.close();
    }

    bit pull()
    {

        if(bitcount==WORDSIZE)
        {
            if(eof)
            {
                std::cerr<<"Reached end of file at an unexpeced point. Check you are decoding a valid file or that the file might be damaged."<<std::endl;
                throw "Reached end of file";
            }
            char c;
            if(!f.get(c)) eof=true;
            buffer = (uint8_t) c;
            bitcount = 0;
        }

        uint8_t mask = 1 << (WORDSIZE-1-bitcount);
        bit b = (mask & buffer);  // obtaining bit
        bitcount++;
        return b;
    }

    uint8_t pullbyte()
    {
        // TODO: Replace with something more efficient
        uint8_t byte = 0;
        for(int i=0; i<WORDSIZE; i++)
        {
            byte = byte << 1;
            byte |= pull();
        }
        return byte;
    }

    const int wordsize = 8;
protected:
    bool eof=false;
    std::ifstream f;
    short bitcount = WORDSIZE;
    uint8_t buffer = 0;
};

}
#endif // BIT_LEVEL_H
