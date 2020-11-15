#include <iostream>

#include "alphabet.h"
#include "file_encoder.h"
#include "file_decoder.h"
#include "input_parsing.h"

#include <string>

std::string ChangeExtension(const std::string & data, const std::string extension);

int main(int argc, char ** argv)
{
    WorkQueue::Pointer wq;
    try {
        wq = WorkQueue::New(argc, argv);
    } catch (...) {
        return 1;
    }

    std::string infile_name;

    /*
     *              ENCODING
     */
    while(wq->NextEncoding(infile_name))
    {
        std::string tempfile_name = ChangeExtension(infile_name,".huf~");
        std::string outfile_name = ChangeExtension(infile_name,".huf");
        try
        {
            Alphabet alph;
            alph.ObtainFrequencies(infile_name);
            alph.BuildTree();
            alph.BuildTable();

            FileEncoder e(alph);
            e.Encode(infile_name, tempfile_name);
            std::cout << "Successfully encoded " << infile_name << std::endl;
            std::rename(tempfile_name.c_str(), outfile_name.c_str());
        }
        catch (...)
        {
            std::cerr << "Failed to encode "<< infile_name << std::endl;
            std::remove(tempfile_name.c_str());
        }
    }

    /*
     *              DECODING
     */

    while(wq->NextDecoding(infile_name))
    {
        std::string tempfile_name = ChangeExtension(infile_name,".tmp~");
        std::string outfile_name = "placeholder.txt";
        try
        {
            FileDecoder d;
            outfile_name = d.Decode(infile_name, tempfile_name);
            std::cout << "Successfully decoded " << infile_name << std::endl;
            std::rename(tempfile_name.c_str(), outfile_name.c_str());
        }
        catch (...)
        {
            std::cerr << "Failed to decode "<< infile_name << std::endl;
            std::remove(tempfile_name.c_str());
        }
    }

    return 0;
}

std::string ChangeExtension(const std::string & data, const std::string extension)
{
    std::string ret = data;
    size_t newlength = ret.find_last_of('.');
    ret.resize(newlength);
    ret += extension;
    return ret;
}


