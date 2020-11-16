#include "file_encoder.h"

FileEncoder::FileEncoder(const std::string & infile_name)
{
    // Reads the tree from the file.
    mpAlphabet = Alphabet::New();
    AddData(infile_name);
    Build();
}

void FileEncoder::AddData(const std::string & infile_name)
{
    mpAlphabet->ObtainFrequencies(infile_name);
}

void FileEncoder::Build()
{
    mpAlphabet->BuildTree();
}


void FileEncoder::Encode(const std::string infile, const std::string outfile) const
{
    std::ifstream fin;
    OpenAndCheck<std::ifstream>(fin,infile);
    BitWriter writer(outfile);

    // Encoding tree

    WriteTree(writer);
    EncodeData(infile, fin, writer);
    writer.eof();
}

void FileEncoder::WriteTree(const std::string & outfile) const
{
    BitWriter writer(outfile);
    WriteTree(writer);
    writer.eof();
}

void FileEncoder::WriteTree(BitWriter & writer) const
{
    if(!mpAlphabet || mpAlphabet->Empty())
    {
        throw "Attempting to store an empty tree";
    }
    if(!mpAlphabet->Built())
    {
        mpAlphabet->BuildTree();
    }
    mpAlphabet->GetRoot()->EncodeRecursive(writer);
}

void FileEncoder::EncodeData(const std::string infile, const std::string outfile) const
{
    std::ifstream fin;
    OpenAndCheck<std::ifstream>(fin,infile);
    BitWriter writer(outfile);
    EncodeData(infile, fin, writer);
    writer.eof();
}


void FileEncoder::EncodeData(const std::string & infile, std::ifstream & fin, BitWriter & writer) const
{
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
}


void FileEncoder::encode_and_print(BitWriter & writer, const unsigned char c) const
{
    std::vector<bool> code = mpAlphabet->EncodeLetter(c);
    for(const bool & bit : code)
    {
        writer.push(bit);
    }
}
