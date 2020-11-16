#ifndef INPUT_PARSING_H
#define INPUT_PARSING_H

#include <iostream>
#include <memory>
#include <list>
#include <string>
#include <sstream>

#define ERROR       0
#define HELP        1
#define ENCODE      2
#define DECODE      3
#define AUTO        4
#define TREE        5
#define SEPARATE    6
#define COMBINED    7


class WorkQueue
{
public:
    typedef std::shared_ptr<WorkQueue> Pointer;
    static Pointer New(int argc, char ** argv){return std::make_shared<WorkQueue>(argc, argv);}

    WorkQueue(int argc, char ** argv);
    int PopEncoding(std::string & infile);
    bool NextCombinedEncoding(std::string & infile);
    bool PopDecoding(std::string & outfile);

    bool external_tree = false;
    bool separate_files = false;
    bool any_combined = false;
    std::string tree_file;
protected:
    std::list<std::string> files_to_encode;
    std::list<std::string> files_to_encode_combined;
    std::list<std::string> files_to_decode;
    std::list<std::string>::iterator curr_comb_file;
};

#endif // INPUT_PARSING_H
