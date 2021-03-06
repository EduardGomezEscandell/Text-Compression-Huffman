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
    OpenAndCheck(f, filename);

    std::string line;
    int totalcount = 0;
    int row = 1;
    while(getline(f, line))
    {
        if(!f.eof())  line += '\n';

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
    empty = false;
    built = false;
}


void Alphabet::BuildTree()
{
    if(mLetters[END_OF_TRANSMISSION]->count == 0)
    {
        mLetters[END_OF_TRANSMISSION]->count = 1;
    }

    MyQueue Q(mLetters);

    while(Q.size() > 1)
    {
        Node::Pointer node1 = Q.pop();
        Node::Pointer node2 = Q.pop();
        Node::Pointer new_node = std::make_shared<Node>(node1, node2);
        Q.push(new_node);
    }
    mRoot = Q.pop();
    mRoot->BuildTableRecursive({});
    built = true;
}

std::string Alphabet::PrintTable()
{
    std::stringstream ss;
    ss << "Char\tASCII\tCount\tCode"<<std::endl<<std::string(40,'-')<<std::endl;
    for(Letter::Pointer & letterptr: mLetters)
    {
        ss << *letterptr;
    }
    return ss.str();
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
        if(letter_ptr->count > 0)
        {
            Q.insert(std::make_shared<Node>(letter_ptr));
        }
    }
}

void Alphabet::Decode(BitReader & reader)
{
    mRoot = std::make_shared<Node>();
    mRoot->DecodeRecursive(reader, mLetters);
    empty = false;
    built = true;
}
