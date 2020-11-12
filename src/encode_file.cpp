#include "encode_file.h"
#include <math.h>

#define END_OF_TRANSMISSION (unsigned char) 0x04

void FileEncoder::encode_and_print(bitstream::writer & writer, const unsigned char c) const
{
    std::vector<bool> code = mpAlphabet->EncodeLetter(c);
    for(const bool & bit : code)
    {
        writer.push(bit);
    }
}

void FileEncoder::Encode(const std::string infile, const std::string outfile) const
{
    std::ifstream fin;
    fin.open(infile);
    if(fin.bad()) throw "Could not open input file";

    bitstream::writer writer(outfile);

    /*
     * Encoding tree
     */

    mpAlphabet->GetRoot()->EncodeRecursive(writer);

    /*
     *  Encoding mesage
     */


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
    writer.eof();
}


void FileDecoder::Decode(const std::string infile, const std::string outfile) const
{
    std::ofstream fout;
    fout.open(outfile);
    if(fout.bad()) throw "Could not open input file";

    bitstream::reader reader(infile);

    mpAlphabet->Decode(reader);

    Node::Weak currnode = mpAlphabet->GetRoot();

    while(true)
    {
        bit nextbit = reader.pull();
        currnode = nextbit ? currnode.lock()->Right : currnode.lock()->Left;
        if(currnode.lock()->Leaf)
        {
            const unsigned char c = currnode.lock()->Data.lock()->value;
            if(c == END_OF_TRANSMISSION) break;
            fout << c;
            currnode = mpAlphabet->GetRoot();
        }
    }
}
