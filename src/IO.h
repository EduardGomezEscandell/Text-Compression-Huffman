#ifndef BITWISEIO_H
#define BITWISEIO_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <exception>

#define WORDSIZE 8
#define END_OF_TRANSMISSION (unsigned char) 0x04

void OpenAndCheck(std::ifstream & f, const std::string & filename);
void OpenAndCheck(std::ofstream & f, const std::string & filename);

size_t last_path_separator(std::string path);
std::string strip_path(const std::string & filename);
std::string extract_path(const std::string & filename);

typedef bool bit;

class BitWriter
{
public:
    BitWriter(const std::string & outfile);
    ~BitWriter();

    void close() {f.close();}
    void push(const bit b);
    void push(const unsigned char ch);
    void fill_byte(bool value);
protected:
    inline void flush();
    std::ofstream f;
    short bitcount = 0;
    uint8_t buffer = 0;
};


class BitReader
{
public:
    BitReader(const std::string & infile);

    void close(){f.close();}
    bit pull();
    uint8_t pullbyte();
    const int wordsize = 8;
protected:
    bool eof=false;
    std::ifstream f;
    short bitcount = WORDSIZE;
    uint8_t buffer = 0;
};

#endif // BIT_LEVEL_H
