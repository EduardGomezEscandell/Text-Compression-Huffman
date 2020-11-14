#include "input_parsing.h"

#define ERROR -1
#define HELP 0
#define ENCODE 1
#define DECODE 2
#define AUTO 3

bool validate(int argc, char ** argv, char ** cursor);
int validate_operation(char * instr);
int auto_get_op(char * input);

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
        int op = validate_operation(*currarg);
        if(op == ERROR) throw_wrong_input();
        if(op == HELP) throw_help();

        ++currarg;

        while(validate(argc,argv,currarg))
        {
            if((*currarg)[0] == '-') break;

            int thisop = op;
            if(op == AUTO) thisop = auto_get_op(*currarg);

            switch(thisop)
            {
            case ENCODE:    files_to_encode.emplace_back(*currarg);    break;
            case DECODE:    files_to_decode.emplace_back(*currarg);    break;
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

int validate_operation(char * input)
{
    if(input[0] != '-' || input[2] != '\0') return ERROR;
    switch(input[1])
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
    case 'a':
        return AUTO;
        break;
    default:
        return ERROR;
    }
}

inline std::string helpmsg()
{
    std::stringstream ss;
    ss << "Use -d to decode and -e to encode followed by the filename(s) of the file(s) you want to opearate on" << std::endl;
    ss << "> huffman [-c|-d] filename1 filename2  ... to perform the same operation on multiple files" << std::endl;
    ss << "> huffman [-c|-d] filename(s) [-c|-d] filename(s) ... to perform multiple operations" << std::endl;
    ss << std::endl;
    ss << "You can also use -a and the program will deduce what operation to perform based on the extension" << std::endl;
    ss << "> huffman -a filename1.txt filename2.huf ... to automatically deduce operation on one or more files" << std::endl;
    ss << std::endl;
    ss << "EXAMPLE using our sample file" << std::endl;
    ss << "> cp tests/humanrights.txt .\n> huffman -c humanrights.txt"<<std::endl;
    return ss.str();
}

void throw_help()
{
    std::cout << "HUFFCODE HELP"<< std::endl << std::endl << helpmsg();
    throw "help";
}

void throw_wrong_input()
{
    std::cerr <<"Wrong input. Use -h to see the help guide"<< std::endl;
    throw "Wrong input";
}

int auto_get_op(char * input)
{
    std::string filename = std::string(input);
    std::string extension = "";
    size_t dotpos =filename.rfind('.');

    if(dotpos == std::string::npos) return ENCODE;

    for(char * ch = &filename[dotpos]; *ch != '\0'; ch++)
    {
        extension += *ch;
    }
    if(extension == ".huf") return DECODE;
    return ENCODE;
}
