#include "encode_file.h"


void FileEncoder::Encode(const std::string infile, const std::string outfile) const
{
    std::ifstream fin;
    fin.open(infile);
    if(fin.bad()) throw "Could not open input file";

    bitstream::writer writer(outfile);

    std::string line;

    while(getline(fin, line))
    {
        if(!fin.eof())  line += '\n';
        for(const unsigned char & c : line)
        {
            std::vector<bool> code = mpAlphabet->EncodeLetter(c);
            for(const bool & bit : code)
            {
                writer.push(bit);
            }
        }
    }
}


void FileDecoder::Decode(const std::string infile, const std::string outfile) const
{
    std::ofstream fout;
    fout.open(outfile);
    if(fout.bad()) throw "Could not open input file";

    bitstream::reader reader(infile);

    Node::Weak currnode = mpAlphabet->GetRoot();

    try {
        while(true)
        {
            bit nextbit = reader.pull();
            currnode = nextbit ? currnode.lock()->Right : currnode.lock()->Left;
            if(currnode.lock()->Leaf)
            {
                fout << currnode.lock()->Data.lock()->value;
                currnode = mpAlphabet->GetRoot();
            }
        }
    } catch (EOFexception e) {

    }
}
