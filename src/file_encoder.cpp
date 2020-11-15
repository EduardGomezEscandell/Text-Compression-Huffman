#include "file_encoder.h"

void FileEncoder::Encode(const std::string infile, const std::string outfile) const
{
    std::ifstream fin;
    OpenAndCheck(fin,infile);
    BitWriter writer(outfile);

    // Encoding tree

    mpAlphabet->GetRoot()->EncodeRecursive(writer);

    // Encoding original filename
    for(const unsigned char & c : strip_path(infile))
    {
        encode_and_print(writer, c);
    }
    encode_and_print(writer, END_OF_TRANSMISSION);

    //  Encoding mesage

    std::string line;
    while(getline(fin, line))
    {
        if(!fin.eof())  line += '\n';

        for(const unsigned char & c : line)
        {
            if(c == END_OF_TRANSMISSION){
                std::cerr<<"Ignoring end of transmission (ASCII 0x04). This is a reserved character."<<std::endl;
            } else {
                encode_and_print(writer, c);
            }
        }
    }
    // Writting an End Of Transmission char at the end
    encode_and_print(writer, END_OF_TRANSMISSION);
    writer.fill_byte(0);
}

void FileEncoder::encode_and_print(BitWriter & writer, const unsigned char c) const
{
    std::vector<bool> code = mpAlphabet->EncodeLetter(c);
    for(const bool & bit : code)
    {
        writer.push(bit);
    }
}
