#ifndef BITWISEIO_H
#define BITWISEIO_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <exception>
#include <memory>

#define WORDSIZE 8
#define END_OF_TRANSMISSION (unsigned char) 0x04

std::string ChangeExtension(const std::string & data, const std::string extension);
size_t last_path_separator(std::string path);
std::string strip_path(const std::string & filename);
std::string extract_path(const std::string & filename);

typedef bool bit;

class BitWriter
{
public:
    BitWriter(const std::string & outfile);
    BitWriter(const BitWriter & rhs) : f(rhs.f), bitcount(rhs.bitcount), buffer(rhs.buffer) {};
    ~BitWriter();

    void close() {f->close();}
    void push(const bit b);
    void push(const unsigned char ch);
    void fill_byte(bool value);
protected:
    inline void flush();
    std::shared_ptr<std::ofstream> f;
    short bitcount = 0;
    uint8_t buffer = 0;
};


class BitReader
{
public:
    BitReader(const std::string & infile);
    BitReader(const BitReader & rhs) : eof(rhs.eof), f(rhs.f), bitcount(rhs.bitcount), buffer(rhs.buffer) {};

    void close(){f->close();}
    bit pull();
    uint8_t pullbyte();
    const int wordsize = 8;
protected:
    void buffer_next_byte();
    bool eof=false;
    std::shared_ptr<std::ifstream> f;
    short bitcount = WORDSIZE;
    uint8_t buffer = 0;
};

template<class T>
void OpenAndCheck(T & f, const std::string & filename)
{
    f.open(filename);
    if(!f.is_open())
    {
        std::cerr << "File " << filename <<" could not be accessed."<<std::endl;
        throw "Could not open input file";
    }
}

template<class T>
void OpenAndCheck(std::shared_ptr<T> & f, const std::string & filename)
{
    f = std::make_shared<T>(filename);
    if(!f->is_open())
    {
        std::cerr << "File " << filename <<" could not be accessed."<<std::endl;
        throw "Could not open input file";
    }
}

#endif // BIT_LEVEL_H
