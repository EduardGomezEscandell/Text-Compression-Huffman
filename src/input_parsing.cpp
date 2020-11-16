#include "input_parsing.h"


bool validate(int argc, char ** argv, char ** cursor);
int validate_operation(char * instr);
int auto_get_op(char * input);

void print_help();
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
        if(op == HELP) print_help();
        if(op == SEPARATE) separate_files = true;

        ++currarg;

        while(validate(argc,argv,currarg))
        {
            if((*currarg)[0] == '-') break;

            int thisop = op;
            if(op == AUTO) thisop = auto_get_op(*currarg);

            switch(thisop)
            {
            case ENCODE:    files_to_encode.emplace_back(*currarg);             break;
            case DECODE:    files_to_decode.emplace_back(*currarg);             break;

            default: std::cerr<<"File "<<*currarg<<" has no specified operation. Use -h for help."<<std::endl;
            }
            ++currarg;
        }
    }
    if(noargs) throw_wrong_input();
}

bool WorkQueue::PopEncoding(std::string & infile)
{
    if(!files_to_encode.empty())
    {
        infile = files_to_encode.front();
        files_to_encode.pop_front();
        return ENCODE;
    }

    return false;
}

bool WorkQueue::PopDecoding(std::string & infile)
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
    case 'a':   return AUTO;
    case 'e':   return ENCODE;
    case 'd':   return DECODE;
    case 'h':   return HELP;
    case 's':   return SEPARATE;
    default:    return ERROR;
    }
}

inline std::string helpmsg()
{
    std::stringstream ss;
    ss << "Use -d to decode and -e to encode followed by the filename(s) of the file(s) you want to opearate on\n";
    ss << "> huffman [-e|-d] filename1 filename2 ...            to perform the same operation on multiple files\n";
    ss << "> huffman [-e|-d] filename(s) [-c|-d] filenames      to perform multiple operations\n";
    ss << "\n";
    ss << "You can also use -a and the program will deduce what operation to perform based on the extension\n";
    ss << "> huffman -a filename1.txt filename2.huf  ...        to automatically deduce operation on one or more files\n";
    ss << "\n";
    ss << "If you want to store the tree and the encoded data in separate files, you can use -s\n";
    ss << "> huffman -s [-e|-d|-a] filename1 filename2 ...      to operate on separate tree(hft) and data(huf) files\n";
    ss << "\n";
    ss << "EXAMPLE using our sample file\n";
    ss << "> cp tests/humanrights.txt .\n> huffman -e humanrights.txt"<<std::endl;
    return ss.str();
}

void print_help()
{
    std::cout << "HUFFCODE HELP"<< std::endl << std::endl << helpmsg();
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
