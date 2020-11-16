# include "IO.h"

std::string ChangeExtension(const std::string & data, const std::string extension)
{
    std::string ret = data;
    size_t newlength = ret.find_last_of('.');
    ret.resize(newlength);
    ret += extension;
    return ret;
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
    OpenAndCheck<std::ofstream>(f, outfile);
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
    /*
     * Pushes 8 bits at once.
     */
    int old_bitcount = bitcount;

    // Writing first half
    buffer = buffer << (WORDSIZE - bitcount);
    uint8_t firsthalf;
    uint8_t mask = (~0) << bitcount;
    firsthalf = ch & mask;
    buffer |= firsthalf >> bitcount;
    flush();

    // Reading second half
    bitcount = old_bitcount;
    mask = ~mask;
    buffer = ch & mask;
}

void BitWriter::eof()
{
    // Pushing zeros until it flushes
    while (bitcount > 0)
    {
        push(false);
    }
    f->close();
}

inline void BitWriter::flush()
{
    *f << buffer;
    buffer = 0;
    bitcount = 0;
}

BitReader::BitReader(const std::string & infile)
{
    OpenAndCheck(f,infile);
};

bit BitReader::pull()
{
    if(bitcount==WORDSIZE){
        buffer_next_byte();
    }

    uint8_t mask = 1 << (WORDSIZE-1-bitcount);
    bit b = (mask & buffer);  // obtaining bit
    bitcount++;
    return b;
}

void BitReader::buffer_next_byte()
{
    if(eof)
    {
        std::cerr<<"Reached end of file at an unexpeced point. Check you are decoding a valid file or that the file might be damaged."<<std::endl;
        throw "Reached end of file";
    }
    char c;
    if(!f->get(c)) eof=true;
    buffer = (uint8_t) c;
    bitcount = 0;
}

uint8_t BitReader::pullbyte()
{
    /*
     * Pulls the next 8 bits, disregarding what actual byte they belong to.
     */
    uint8_t mask;
    int old_bitcount = bitcount;

    // Reading first half
    uint8_t firsthalf;
    mask = ~0;
    mask = mask >> bitcount;
    firsthalf = buffer & mask;
    firsthalf = firsthalf << bitcount;

    // Reading second half
    uint8_t secondhalf;
    buffer_next_byte();
    bitcount = old_bitcount;
    mask = ~mask;
    secondhalf = buffer & mask;
    secondhalf = secondhalf >> (WORDSIZE -bitcount);

    return firsthalf | secondhalf;
}


