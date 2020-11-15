#ifndef ALPHABET_H
#define ALPHABET_H

#include <iostream>
#include <sstream>
#include <algorithm>
#include <memory>

#include "node.h"

struct compare
{
    bool operator()(const Node::Pointer & LHS, const Node::Pointer & RHS) const;
};

class MyQueue{
public:
    MyQueue(std::array<Letter::Pointer, 256> & letters);
    Node::Pointer pop();
    void push(Node::Pointer & newitem);
    size_t size() const {return Q.size();}
protected:
    std::multiset<Node::Pointer, compare> Q;
};

class Alphabet
{
public:
    typedef std::shared_ptr<Alphabet> Pointer;
    static Pointer New(){return std::make_shared<Alphabet>();}
    Alphabet();

    void Decode(BitReader & reader);

    void ObtainFrequencies(std::string filename);
    void BuildTree();
    void BuildTable() {mRoot->BuildTableRecursive({});}

    Node::Pointer & GetRoot() {return mRoot;}

    std::vector<bool> EncodeLetter(const char & c) const {return mLetters[(uint8_t) c]->code;}

    std::string PrintTree() {return mRoot->PrintTreeRecursive({});}
    std::string PrintTable();
protected:
    std::array<Letter::Pointer, 256> mLetters;
    Node::Pointer mRoot;
};

#endif // ALPHABET_H
