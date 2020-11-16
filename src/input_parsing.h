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
#define SEPARATE    5


class WorkQueue
{
public:
    typedef std::shared_ptr<WorkQueue> Pointer;
    static Pointer New(int argc, char ** argv){return std::make_shared<WorkQueue>(argc, argv);}

    WorkQueue(int argc, char ** argv);
    bool PopEncoding(std::string & infile);
    bool PopDecoding(std::string & outfile);

    bool separate_files = false;
protected:
    std::list<std::string> files_to_encode;
    std::list<std::string> files_to_decode;
};

#endif // INPUT_PARSING_H
