#include "input_parsing.h"

#define ERROR 0
#define ENCODE 1
#define DECODE 2
#define HELP 3

bool validate(int argc, char ** argv, char ** cursor);
int validate_instruction(char * instr);

void throw_help();
void throw_wrong_input();

WorkQueue::WorkQueue(int argc, char ** argv)
{
    char ** currarg = argv;
    currarg++;

    bool noargs = true;
    while(validate(argc,argv,currarg))
    {
        noargs = false;
        int instr = validate_instruction(*currarg);
        if(instr == ERROR) throw_wrong_input();
        if(instr == HELP) throw_help();
        ++currarg;
        while(validate(argc,argv,currarg))
        {
            if((*currarg)[0] == '-') break;
            switch(instr)
            {
            case ENCODE:    files_to_encode.push_back(std::string(*currarg));    break;
            case DECODE:    files_to_decode.push_back(std::string(*currarg));    break;
            }
            ++currarg;
        }

    }
    if(noargs) throw_wrong_input();
}

bool WorkQueue::NextEncoding(std::string & infile)
{
    if(files_to_encode.size() == 0) return false;
    infile = files_to_encode.front();
    files_to_encode.pop_front();
    return true;
}

bool WorkQueue::NextDecoding(std::string & infile)
{
    if(files_to_decode.size() == 0) return false;
    infile = files_to_decode.front();
    files_to_decode.pop_front();
    return true;
}

inline bool validate(int argc, char ** argv, char ** cursor)
{
    return (cursor - argv < argc);
}

int validate_instruction(char * instr)
{
    if(instr[0] != '-' || instr[2] != '\0') return ERROR;
    switch(instr[1])
    {
    case 'e':
        return ENCODE;
        break;
    case 'd':
        return DECODE;
        break;
    case 'h':
        return HELP;
        break;
    default:
        return ERROR;
    }
}

inline std::string helpmsg()
{
    std::stringstream ss;
    ss << "Use -d to decode and -e to encode followed by the filename(s) of the file(s) you want to opearate on" << std::endl;
    ss << "> huffman [-c|-d] filename1 filename2  ... to perform the same operation to multiple files" << std::endl;
    ss << "> huffman [-c|-d] filename(s) [-c|-d] filename(s) ... to perform multiple operations" << std::endl;
    ss << "EXAMPLE using our sample file" << std::endl;
    ss << "> cp tests/humanrights.txt .\n> huffman -c humanrights.txt"<<std::endl;
    return ss.str();
}

void throw_help()
{
    std::cout << "HUFFCODE HELP"<< std::endl << helpmsg();
    throw "help";
}

void throw_wrong_input()
{
    std::cerr <<"Wrong input. Use -h to see the help guide"<< std::endl;
    throw "Wrong input";
}
