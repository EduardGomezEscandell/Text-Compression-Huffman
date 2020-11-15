#ifndef FILE_ENCODER_H
#define FILE_ENCODER_H

#include <string>
#include <fstream>

#include "alphabet.h"
#include "IO.h"


class FileEncoder
{
public:
    typedef std::shared_ptr<FileEncoder> Pointer;

    FileEncoder() {Alphabet::New();};
    FileEncoder(const std::string & infile_name);
    FileEncoder(Alphabet::Pointer & pAlphabet) : mpAlphabet(pAlphabet) {};
    FileEncoder(Alphabet & rAlphabet) : mpAlphabet(std::make_shared<Alphabet>(rAlphabet)) {};

    void AddData(const std::string & infile_name);
    void Build();

    void Encode(const std::string infile, const std::string outfile) const;

    void WriteTree(const std::string & outfile) const;
    void WriteTree(BitWriter & writer) const;

    void EncodeData(const std::string infile, const std::string outfile) const;
    void EncodeData(const std::string & infile, std::ifstream & fin, BitWriter & writer) const;
protected:
    void encode_and_print(BitWriter & writer, const unsigned char c) const;
    Alphabet::Pointer mpAlphabet = nullptr;
};

#endif // FILE_ENCODER_H
