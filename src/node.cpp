#include "node.h"

std::string Node::PrintTreeRecursive(std::vector<bool> lines)
{
    std::stringstream ss;
    ss << *this;
    bool children = false;
    if(Left)
    {
        for(bool l : lines)
        {
            ss <<(l ? "   │" : "    ");
        }
        std::string connector = Right ? "   ├─" : "   └─";
        auto thislines = lines;
        thislines.push_back(true);
        ss << connector << Left->PrintTreeRecursive(thislines);
        children = true;
    }
    if(Right)
    {
        for(bool l : lines)
        {
            ss << (l ? "   │" : "    ");
        }
        auto thislines = lines;
        thislines.push_back(false);
        ss << "   └─" << Right->PrintTreeRecursive(thislines);
        children = true;
    }
    return ss.str();
}


void Node::BuildTableRecursive(std::vector<bool> code)
{
    if(Leaf)
    {
        Data.lock()->code = std::vector<bool>(code);
    }
    if(Left)
    {
        std::vector<bool> codechild = code;
        codechild.push_back(false);
        Left->BuildTableRecursive(codechild);
    }
    if(Right)
    {
        std::vector<bool> codechild = code;
        codechild.push_back(true);
        Right->BuildTableRecursive(codechild);
    }
}

std::ostream& operator<<(std::ostream& os, const Letter & letter)
{

    if(letter.value > 31){
        os<<"["<<letter.value <<"]"<< "\t(" << (int) letter.value << ")";
    } else {
        os<< "[�]\t(" << (int) letter.value << ")";
    }
    os<<"\t"<< letter.count <<"\t";
    for(const bool & b : letter.code)
    {
        os << b;
    }
    os << std::endl;
    return os;
}

std::ostream& operator<<(std::ostream & os, const Node & node)
{
    if(node.Leaf)
    {
        Letter::Pointer data = node.Data.lock();
        const unsigned char ch = data->value>31 ? data->value : '*';

        os << "c:" << ch <<"("<< (int)ch<<")"<<" f:"<<data->count;


    } else {
        os << "nac"; // <<" w:"<<node.Weight;
    }
    os<<std::endl;
    return os;
}
