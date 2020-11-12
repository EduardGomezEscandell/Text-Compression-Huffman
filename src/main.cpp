#include <iostream>

#include "alphabet.h"
#include "encode_file.h"

void print_help();

int main(int argc, char ** argv)
{
    if(argc != 3){
        print_help();
        return 1;
    }
    std::string infile_name = argv[1];
    std::string outfile_name = argv[2];

    // Building tree
    Alphabet alph;
    alph.ObtainFrequencies(infile_name);
    alph.BuildTree();
    alph.BuildTable();

    // Encoding file with tree
    FileEncoder e(alph);
    e.Encode(infile_name, "results/encoded.huf");

    // Recovering tree from file, and re-translating
    FileDecoder d;
    d.Decode("results/encoded.huf", outfile_name);

    return 0;
}


void print_help()
{
    std::cerr << "Wrong number of arguments. It should be two"<<std::endl;
    std::cerr << "huffcode infile outfile"<<std::endl;
}
