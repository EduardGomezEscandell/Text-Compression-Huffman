#ifndef BIT_LEVEL_H
#define BIT_LEVEL_H

#define WORDSIZE 8

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <exception>

typedef bool bit;


class EOFexception : std::exception
{
public:
    const char* what() const throw() {return "Reached end of file";}
};


namespace  bitstream {

class writer
{
public:
    writer(const std::string & outfile) {
        f.open(outfile);
        if(!f.is_open()) throw "Could not open output file";
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

    void eof()
    {
        // Pushing zeros until it flushes
        while (bitcount > 0) {
            push(0);
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
        f.open(infile);
        if(!f.is_open()) throw "Could not open output file";
    };

    void close()
    {
        f.close();
    }

    bit pull()
    {
        if(mask==0)
        {
            if(eof) throw EOFexception();
            char c;
            if(!f.get(c)) eof=true;
            buffer = (uint8_t) c;
            mask = 1 << (WORDSIZE-1);
        }
        bit b = (mask & buffer) != 0;  // obtaining bit
        mask = mask >> 1;                   // Shifting right
        return b;
    }
    const int wordsize = 8;
protected:
    bool eof=false;
    std::ifstream f;
    uint8_t mask = 0;
    uint8_t buffer = 0;
};

}
#endif // BIT_LEVEL_H
