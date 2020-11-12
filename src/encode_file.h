#ifndef ENCODEFILE_H
#define ENCODEFILE_H

#include <string>
#include <fstream>

#include "alphabet.h"
#include "bit_level.h"

class FileEncoder
{
public:
    FileEncoder(Alphabet::Pointer & pAlphabet) : mpAlphabet(pAlphabet) {};
    FileEncoder(Alphabet & rAlphabet) : mpAlphabet(std::make_shared<Alphabet>(rAlphabet)) {};
    void Encode(const std::string infile, const std::string outfile) const;
protected:
    void encode_and_print(bitstream::writer & writer, const unsigned char c) const;
    Alphabet::Pointer mpAlphabet;
};


class FileDecoder
{
public:
    FileDecoder(Alphabet::Pointer & pAlphabet) : mpAlphabet(pAlphabet) {};
    FileDecoder(Alphabet & rAlphabet) : mpAlphabet(std::make_shared<Alphabet>(rAlphabet)) {};
    void Decode(const std::string infile, const std::string outfile) const;
protected:
    Alphabet::Pointer mpAlphabet;
};

#endif // ENCODEFILE_H
