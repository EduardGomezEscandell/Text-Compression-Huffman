# include "IO.h"

void OpenAndCheck(std::ifstream & f, const std::string & filename)
{
    f.open(filename);
    if(!f.is_open())
    {
        std::cerr << "File " << filename <<" could not be accessed."<<std::endl;
        throw "Could not open input file";
    }
}

void OpenAndCheck(std::ofstream & f, const std::string & filename)
{
    f.open(filename);
    if(!f.is_open())
    {
        std::cerr << "File " << filename <<" could not be accessed."<<std::endl;
        throw "Could not open input file";
    }
}

size_t last_path_separator(std::string path)
{
    // Linux
    size_t lastslash = path.find_last_of('/');
    // Windows
    if(lastslash == std::string::npos)
    {
        lastslash = path.find_last_of('\\');
    }

    if(lastslash == std::string::npos)
    { // Filename contains no path
        return -1;
    }
    return lastslash;
}

std::string strip_path(const std::string & filename)
{
    size_t lastslash = last_path_separator(filename);
    const char * c = &filename[lastslash+1];
    std::string ret = c;
    return ret;
}

std::string extract_path(const std::string & filename)
{
    size_t lastpath = last_path_separator(filename);
    std::string ret = filename;
    ret.resize(lastpath+1);
    return ret;
}

BitWriter::BitWriter(const std::string & outfile) {
    OpenAndCheck(f,outfile);
}

BitWriter::~BitWriter()
{
    if(bitcount != 0)
    std::cerr<<"[WARNING] There were unprinted bits in the buffer. Use BitWriter::eof() to print them before destroying the BitWriter." <<std::endl;
}

void BitWriter::push(const bit b)
{
    buffer = buffer << 1;
    buffer |= b;
    bitcount++;
    if(bitcount == WORDSIZE) flush();
}


void BitWriter::push(const unsigned char ch)
{
    // TODO Replace with something more efficient
    uint8_t mask = 1 << (WORDSIZE-1);
    while(mask)
    {
        bit b = ch & mask;
        push(b);
        mask = mask >> 1;
    }

}

void BitWriter::fill_byte(bool value)
{
    // Pushing zeros until it flushes
    while (bitcount > 0)
    {
        push(value);
    }
}

inline void BitWriter::flush()
{
    f << buffer;
    buffer = 0;
    bitcount = 0;
}

BitReader::BitReader(const std::string & infile)
{
    OpenAndCheck(f,infile);
};

bit BitReader::pull()
{

    if(bitcount==WORDSIZE)
    {
        if(eof)
        {
            std::cerr<<"Reached end of file at an unexpeced point. Check you are decoding a valid file or that the file might be damaged."<<std::endl;
            throw "Reached end of file";
        }
        char c;
        if(!f.get(c)) eof=true;
        buffer = (uint8_t) c;
        bitcount = 0;
    }

    uint8_t mask = 1 << (WORDSIZE-1-bitcount);
    bit b = (mask & buffer);  // obtaining bit
    bitcount++;
    return b;
}

uint8_t BitReader::pullbyte()
{
    // TODO: Replace with something more efficient
    uint8_t byte = 0;



    for(int i=0; i<WORDSIZE; i++)
    {
        byte = byte << 1;
        byte |= pull();
    }
    return byte;
}


