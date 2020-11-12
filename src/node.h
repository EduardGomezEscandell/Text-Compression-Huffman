#ifndef NODE_H
#define NODE_H

#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <memory>
#include "bit_level.h"

class Letter{
public:
    typedef std::shared_ptr<Letter> Pointer;
    typedef std::weak_ptr<Letter> Weak;

    Letter(unsigned char v) : value(v) {};
    Letter(const Letter & rhs) : count(rhs.count), value(rhs.value) {};

    void operator+=(const int & rhs){count+=rhs;};
    int count=0;
    unsigned char value = '\0';
    std::vector<bool> code;
};
std::ostream& operator<<(std::ostream& os, const Letter & letter);


class Node{
public:
    typedef std::shared_ptr<Node> Pointer;
    typedef std::weak_ptr<Node> Weak;

    Node(Node::Pointer & L, Node::Pointer & R) : Left(L) , Right(R) {Weight = L->Weight + R->Weight;}
    Node(Letter::Pointer & letter) : Leaf(true) , Data(letter) {Weight = letter->count;}

    void BuildTableRecursive(std::vector<bool> code);
    std::string PrintTreeRecursive(std::vector<bool> lines);

    bool Leaf = false;
    double Weight;

    void EncodeNodeRecursive() const;

    Node::Pointer Left = nullptr;
    Node::Pointer Right = nullptr;
    Letter::Weak Data;
};
std::ostream& operator<<(std::ostream& os, const Node & node);

#endif // NODE_H
