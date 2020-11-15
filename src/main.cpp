#include <iostream>

#include "alphabet.h"
#include "file_encoder.h"
#include "file_decoder.h"
#include "input_parsing.h"

#include <string>

std::string ChangeExtension(const std::string & data, const std::string extension);

int main(int argc, char ** argv)
{
    /*
     *              INTERPRETATION OF ARGUMENTS
     */
    WorkQueue::Pointer wq;
    try {
        wq = WorkQueue::New(argc, argv);
    } catch (...) {
        return 1;
    }

    std::string infile_name;
    FileEncoder::Pointer encoder;
    FileDecoder::Pointer decoder;

    /*
     *              READING TREE
     */


    try {
        if(!wq->self_code)
        {
            encoder = std::make_shared<FileEncoder>(wq->tree_file);
            decoder = std::make_shared<FileDecoder>(wq->tree_file);
        }
    } catch (...) {
        std::cerr << "Failed to read tree"<<std::endl;
        return 1;
    }

    /*
     *              COMBINED ENCODING
     */
    while(wq->NextCombinedEncoding(infile_name))
    {
        encoder->AddData(infile_name);
    }

    /*
     *              ENCODING
     */
    int op;
    std::string treefile_name = "combined.hft";
    std::string treefile_name_tmp = "combined.hft~";
    while((op = wq->PopEncoding(infile_name)))
    {
        std::string tempfile_name = ChangeExtension(infile_name,".huf~");
        std::string outfile_name = ChangeExtension(infile_name,".huf");
        if(op == SEPARATE){
            treefile_name = ChangeExtension(infile_name,".hft~");
            treefile_name_tmp = ChangeExtension(infile_name,".hft");
        }

        try
        {
            if(op != COMBINED){
                encoder = std::make_shared<FileEncoder>(infile_name);
            }

            if(wq->self_code){
                encoder->Encode(infile_name, tempfile_name);
            } else {
                encoder->WriteTree(treefile_name_tmp);
                encoder->EncodeData(infile_name, tempfile_name);
            }

            std::cout << "Successfully encoded " << infile_name << std::endl;
            if(wq->self_code)
                std::rename(treefile_name_tmp.c_str(), treefile_name.c_str());
            std::rename(tempfile_name.c_str(), outfile_name.c_str());
        }
        catch (...)
        {
            std::cerr << "Failed to encode "<< infile_name << std::endl;
            std::remove(tempfile_name.c_str());
        }
        op = wq->PopEncoding(infile_name);
    }

    /*
     *              DECODING
     */

    while(wq->PopDecoding(infile_name))
    {
        std::string tempfile_name = ChangeExtension(infile_name,".tmp~");
        std::string outfile_name = "placeholder.txt";
        try
        {
            if(wq->self_code)
            {
                decoder = std::make_shared<FileDecoder>();
            }
            outfile_name = decoder->Decode(infile_name, tempfile_name);
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


