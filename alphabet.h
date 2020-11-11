#ifndef ALPHABET_H
#define ALPHABET_H

#include <iostream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <set>
#include <memory>
#include <functional>
#include <fstream>
#include <algorithm>

class Letter{
public:
    typedef std::shared_ptr<Letter> Pointer;
    typedef std::weak_ptr<Letter> Weak;

    Letter(unsigned char v, int c) : count(c), value(v) {};
    Letter(const Letter & rhs) : count(rhs.count), value(rhs.value), freq(rhs.freq){};

    void operator+=(const int & rhs){count+=rhs;};
    int count=0;
    unsigned char value = '\0';
    double freq = 0;
    std::vector<bool> code;
};

class Node{
public:
    typedef std::shared_ptr<Node> Pointer;
    typedef std::weak_ptr<Node> Weak;

    Node(Node::Pointer & L, Node::Pointer & R) : Left(L) , Right(R), Weight(L->Weight + R->Weight) {};
    Node(Letter::Pointer & letter) : Leaf(true) , Data(letter), Weight(letter->freq) {};

    void BuildTableRecursive(std::vector<bool> code);
    std::string PrintTableRecursive();
    std::string PrintTreeRecursive(std::vector<bool> lines);

    bool Leaf = false;
    double Weight;

    Node::Pointer Left = nullptr;
    Node::Pointer Right = nullptr;
    Letter::Weak Data;
};
std::ostream& operator<<(std::ostream& os, const Node & node);

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
    Alphabet();

    void ObtainFrequencies(std::string filename);
    void BuildTree();
    void BuildTable() {mRoot->BuildTableRecursive({});}

    Node::Pointer & GetRoot() {return mRoot;}

    std::vector<bool> EncodeLetter(const char & c) {return mLetters[(int) c]->code;};

    std::string PrintTree() {return mRoot->PrintTreeRecursive({});}
    std::string PrintTable() {return mRoot->PrintTableRecursive();}
protected:
    std::array<Letter::Pointer, 256> mLetters;
    Node::Pointer mRoot;
};

#endif // ALPHABET_H
