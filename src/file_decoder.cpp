#include "file_decoder.h"

std::string FileDecoder::Decode(const std::string & infile, const std::string & outfile) const
{
    std::ofstream fout;
    OpenAndCheck(fout,outfile);
    BitReader reader(infile);

    // Decoding alphabet
    mpAlphabet->Decode(reader);

    // Decoding filename
    std::stringstream ss;
    while(true)
    {
        unsigned char c = decode_next_char(reader);
        if(c == END_OF_TRANSMISSION) break;
        ss << c;
    }
    std::string decoded_name = extract_path(infile) + ss.str();

    // Decoding data
    while(true)
    {
        unsigned char c = decode_next_char(reader);
        if(c == END_OF_TRANSMISSION) break;
        fout << c;
    }
    return  decoded_name;
}

unsigned char FileDecoder::decode_next_char(BitReader & reader) const
{
    Node::Weak currnode = mpAlphabet->GetRoot();
    while(true) // iterating bit per bit
    {
        bit nextbit = reader.pull();
        currnode = nextbit ? currnode.lock()->Right : currnode.lock()->Left;

        if(!currnode.lock())
        {
            std::cerr << "The decoded tree does not match the coded data. Are you sure this is a huf file?" << std::endl;
            throw "Wrong encoding";
        }

        if(currnode.lock()->Leaf)
        {
            const unsigned char c = currnode.lock()->Data.lock()->value;
            return c;
        }
    }
}



