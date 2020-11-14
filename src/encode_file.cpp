#include "encode_file.h"
#include <math.h>

#define END_OF_TRANSMISSION (unsigned char) 0x04

std::string strip_path(const std::string & filename);
std::string extract_path(const std::string & filename);

void FileEncoder::encode_and_print(bitstream::writer & writer, const unsigned char c) const
{
    std::vector<bool> code = mpAlphabet->EncodeLetter(c);
    for(const bool & bit : code)
    {
        writer.push(bit);
    }
}

unsigned char FileDecoder::decode_next_char(bitstream::reader & reader) const
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

void FileEncoder::Encode(const std::string infile, const std::string outfile) const
{
    std::ifstream fin;
    OPEN(fin,infile);
    bitstream::writer writer(outfile);

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
    writer.eof();
}


std::string FileDecoder::Decode(const std::string & infile, const std::string & outfile) const
{
    std::ofstream fout;
    OPEN(fout,outfile);
    bitstream::reader reader(infile);

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


size_t last_path_separator(std::string path)
{
    // Linux
    size_t lastslash = path.find_last_of('/');
    // Windows
    if(lastslash == std::string::npos)
    {
        lastslash = path.find_last_of('\\');
    }

    if(lastslash == std::string::npos)
    { // Filename contains no path
        return -1;
    }
    return lastslash;
}

std::string strip_path(const std::string & filename)
{
    size_t lastslash = last_path_separator(filename);
    const char * c = &filename[lastslash+1];
    std::string ret = c;
    return ret;
}

std::string extract_path(const std::string & filename)
{
    size_t lastpath = last_path_separator(filename);
    std::string ret = filename;
    ret.resize(lastpath+1);
    return ret;
}
