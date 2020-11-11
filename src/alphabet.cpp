#include "alphabet.h"

Alphabet::Alphabet()
{
    for(int i=0; i<256; i++)
    {
        mLetters[i] = std::make_shared<Letter>((unsigned char)i);
    }
}

void Alphabet::ObtainFrequencies(std::string filename)
{
    std::ifstream f;
    f.open(filename);

    if(f.bad()) throw "Could not open input file";

    std::string line;
    int totalcount = 0;
    int row = 1;
    while(getline(f, line))
    {
        int col = 1;
        for(unsigned char c: line)
        {
            if(uint8_t(c) > 255 || uint8_t(c) < 0)
            {
                std::cerr<<"Only ascii characters allowed! Character "<< c <<" ("<<(uint8_t)c<<") will be ignored. See R"<<row<<"C"<<col<<std::endl;
            } else {
                *mLetters.at((uint8_t)c)+=1;
                totalcount++;
            }
            col++;
        }
        row++;
    }
    f.close();
    for(std::shared_ptr<Letter> & l : mLetters)
    {
        l->freq = l->count / (1.0 * totalcount);
    }
}


void Alphabet::BuildTree()
{
    MyQueue Q(mLetters);

    while(Q.size() > 1)
    {
        Node::Pointer node1 = Q.pop();
        Node::Pointer node2 = Q.pop();
        Node::Pointer new_node = std::make_shared<Node>(node1, node2);
        Q.push(new_node);
    }
    mRoot = Q.pop();
}



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

std::string Node::PrintTableRecursive()
{
    std::stringstream ss;
    if(Leaf)
    {
        Letter & l = *Data.lock();

        if((int) l.value > 31){
            ss<< l.value << " (" << (int) l.value << ")";
        } else {
            ss<< "[] (" << (int) l.value << ")";
        }
        ss<<"\t";
        if((int) l.value > -10) ss<<"\t"; // For alignment purposes

        for(const bool & b : l.code)
        {
            ss << b;
        }
        ss << std::endl;
    }
    if(Left)
    {
        ss << Left->PrintTableRecursive();
    }
    if(Right)
    {
        ss << Right->PrintTableRecursive();
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

std::ostream& operator<<(std::ostream & os, const Node & node)
{
    if(node.Leaf)
    {
        Letter::Pointer data = node.Data.lock();
        unsigned char ch = data->value>31 ? data->value : '*';

        os << "c:" << ch <<"("<< (uint8_t)ch<<")"<<" f:"<<data->freq;


    } else {
        os << "nac"; // <<" w:"<<node.Weight;
    }
    os<<std::endl;
    return os;
}

bool compare::operator()(const Node::Pointer & LHS, const Node::Pointer & RHS) const
{
    // Must return element that will be placed in front of queue
    return LHS->Weight < RHS->Weight;
}

Node::Pointer MyQueue::pop()
{
    auto it = Q.begin();
    Node::Pointer w = *it;
    Q.erase(it);
    return w;
}

void MyQueue::push(Node::Pointer & newitem)
{
    Q.insert(newitem);
}

MyQueue::MyQueue(std::array<Letter::Pointer, 256> & letters)
{
    for(auto & letter_ptr : letters)
    {
        Q.insert(std::make_shared<Node>(letter_ptr));
    }
}
