#ifndef INPUT_PARSING_H
#define INPUT_PARSING_H

#include <iostream>
#include <memory>
#include <list>
#include <string>
#include <sstream>

class WorkQueue
{
public:
    typedef std::shared_ptr<WorkQueue> Pointer;
    static Pointer New(int argc, char ** argv){return std::make_shared<WorkQueue>(argc, argv);}

    WorkQueue(int argc, char ** argv);
    bool NextEncoding(std::string & infile);
    bool NextDecoding(std::string & outfile);
protected:
    std::list<std::string> files_to_encode;
    std::list<std::string> files_to_decode;
};

#endif // INPUT_PARSING_H
