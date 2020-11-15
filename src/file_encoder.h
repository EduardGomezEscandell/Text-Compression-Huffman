#ifndef FILE_ENCODER_H
#define FILE_ENCODER_H

#include <string>
#include <fstream>

#include "alphabet.h"
#include "IO.h"

class FileEncoder
{
public:
    FileEncoder();
    FileEncoder(const std::string & infile_name);
    FileEncoder(Alphabet::Pointer & pAlphabet) : mpAlphabet(pAlphabet) {};
    FileEncoder(Alphabet & rAlphabet) : mpAlphabet(std::make_shared<Alphabet>(rAlphabet)) {};
    void Encode(const std::string infile, const std::string outfile) const;
protected:
    void encode_and_print(BitWriter & writer, const unsigned char c) const;
    Alphabet::Pointer mpAlphabet;
};

#endif // FILE_ENCODER_H
