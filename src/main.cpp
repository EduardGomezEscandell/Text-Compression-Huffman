#include <iostream>

#include "alphabet.h"
#include "encode_file.h"
#include "input_parsing.h"

#include <string>

std::string data2results(const std::string & data, const std::string extension);

int main(int argc, char ** argv)
{
    WorkQueue::Pointer wq;
    try {
        wq = WorkQueue::New(argc, argv);
    } catch (...) {
        return 1;
    }

    std::string infile_name;

    // Building tree
    while(wq->NextEncoding(infile_name))
    {
        try {
            std::string outfile_name = data2results(infile_name,".huf");
            Alphabet alph;
            alph.ObtainFrequencies(infile_name);
            alph.BuildTree();
            alph.BuildTable();

            FileEncoder e(alph);
            e.Encode(infile_name, outfile_name);
            std::cout << "Succesfully encoded " << infile_name << std::endl;
        } catch (...) {
            std::cerr << "Failed to encode "<< infile_name << std::endl;
        }
    }

    while(wq->NextDecoding(infile_name))
    {
        try {
            std::string outfile_name = data2results(infile_name,".txt");
            FileDecoder d;
            d.Decode(infile_name, outfile_name);
            std::cout << "Succesfully decoded " << infile_name << std::endl;
        } catch (...) {
            std::cerr << "Failed to decode "<< infile_name << std::endl;
        }
    }

    return 0;
}

std::string data2results(const std::string & data, const std::string extension)
{
    std::string ret = data;
    size_t newlength = ret.find_last_of('.');
    ret.resize(newlength);
    ret += extension;

    return ret;
}


