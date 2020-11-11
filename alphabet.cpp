#include "alphabet.h"

Alphabet::Alphabet()
{
    for(int i=0; i<256; i++)
    {
        mLetters[i] = std::make_shared<Letter>(char(i), i);
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
        for(char & c: line)
        {
            if(int(c) > 255 || int(c) < 0)
            {
                std::cerr<<"Only ascii characters allowed! Character "<< c <<" ("<<(int)c<<") will be ignored. See R"<<row<<"C"<<col<<std::endl;
            } else {
                *mLetters.at((int)c)+=1;
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



std::string Node::StrRecursive(std::vector<bool> lines)
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
        ss << connector << Left->StrRecursive(thislines);
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
        ss << "   └─" << Right->StrRecursive(thislines);
        children = true;
    }
    return ss.str();
}

std::string Node::TableRecursive(std::string code)
{
    std::stringstream ss;
    if(Leaf)
    {
        char c = Data.lock()->value;
        if((int)c < 31)
        {
            ss << code << " : " << "CNTRL ("<<(int) c <<")" << std::endl;
        } else {
            ss << code << " :   " << c << "   ("<<(int) c <<")" << std::endl;
        }
        return ss.str();
    }
    if(Left)
    {
        ss << Left->TableRecursive(code + '0');
    }
    if(Right)
    {
        ss << Right->TableRecursive(code + '1');
    }
    return ss.str();
}

std::ostream& operator<<(std::ostream & os, const Node & node)
{
    if(node.Leaf)
    {
        Letter::Pointer data = node.Data.lock();
        char ch = data->value>31 ? data->value : '*';

        os << "c:" << ch <<"("<< (int)ch<<")"<<" f:"<<data->freq;


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
