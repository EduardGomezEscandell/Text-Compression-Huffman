#ifndef FILE_DECODER_H
#define FILE_DECODER_H

#include <string>
#include <fstream>

#include "alphabet.h"
#include "IO.h"

class FileDecoder
{
public:
    typedef std::shared_ptr<FileDecoder> Pointer;

    FileDecoder() {mpAlphabet = std::make_shared<Alphabet>();}
    FileDecoder(const std::string & treefile_name);
    FileDecoder(Alphabet::Pointer & pAlphabet) : mpAlphabet(pAlphabet) {}
    FileDecoder(Alphabet & rAlphabet) : mpAlphabet(std::make_shared<Alphabet>(rAlphabet)) {}
    std::string Decode(const std::string & infile, const std::string & outfile) const;
protected:
    unsigned char decode_next_char(BitReader & reader) const;
    Alphabet::Pointer mpAlphabet = nullptr;
};

#endif // FILE_DECODER_H
