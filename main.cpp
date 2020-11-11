#include <iostream>

#include "alphabet.h"

using namespace std;

void print_help();

int main(int argc, char ** argv)
{
    if(argc != 3){
        print_help();
        return 1;
    }
    std::string infile_name = argv[1];
    std::string outfile_name = argv[2];

    Alphabet alph;
    alph.ObtainFrequencies(infile_name);
    alph.BuildTree();

    std::ofstream f;
    f.open(outfile_name);
    f << alph.Table();
    f.close();

    std::cout << alph.Tree();

    return 0;
}


void print_help()
{
    std::cerr << "Wrong number of arguments. It should be two"<<std::endl;
    std::cerr << "huffcode infile outfile"<<std::endl;
}
