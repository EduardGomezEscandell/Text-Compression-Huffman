# Huffman Coding

Implementing Huffman coding as a recreational project. Still in early stages. Huffman coding is well explained in the [wikipedia page](https://en.wikipedia.org/wiki/Huffman_coding), but as a quick explanation:
- It is a lossless text compression algorithm.
- Works by creating an unbalanced binary tree with all characters at the leafs.
- Each characters' code is its adress in the tree.
- More frequent characters are higher up in the tree, therefore have shorter codes.

In ASCII each letter takes 8 bits. Once encoded, the most frequent characters take 3 bits while the least ones take 22. Note that most of the latter will be control characters that seldom, if ever, appear. The set of characters can be restricted to make it more efficient. The tree can be constructed on a per-file basis or use generic precompiled values, e.g. the optimal for the english language.

## Progress report
So far the program:
- Reads a file and counts the frequency of each ASCII character
- Builds a huffmann tree
- Encodes a text and stores it
- Reads the encoded file and recovers the original text

The program doesn't yet:
- Store the tree in the encoded file or separately.
- Encode multiple files with the same tree.
- Use a snippet of text to build the tree instead of the whole text.
- Allow for unicode characters.
- Perform lossless compression (see next section).

## Current bugs
So far the encoding is not perfect. An attempt to encode the Universal Declaration of Human Rights yields:
```
.../HuffmanCoding$ diff data/humanrights.txt results/hr.txt
155c155
< Nothing in this Declaration may be interpreted as implying for any State, group
or person any right to engage in any activity or to perform any act aimed at the 
destruction of any of the rights and freedoms set forth herein.
\ No newline at end of file
---
> Nothing in this Declaration may be interpreted as implying for any State, group
or person any right to engage in any activity or to perform any act aimed at the 
destruction of any of the rights and freedoms set forth herein
\ No newline at end of file
```
As you can see the last character is missing (a dot/full stop). However that is the only error, so the results seem promissing. This text contains 10850 characters or bytes, and gets reduced to 6281 bites, a 57.9% of the original. This could be further improved by excluding characters with zero frequency from the huffmann tree, or applying other natural text compression alorithms beforehand.


## How to use
First clone the repository and, on linux, run `sh build.sh`. On windows you're on your own but it is a small project so it shouldn't be difficult.

A symbolic link will be created in the repository directory, as well as a results directory. You can run a command such as: `huffman file1.txt file2.txt`. The first file will be your input file. The second one is the output. There will also appear a  `results/encoded.txt`, with the encoded file. The program will perform the following process:
```
file1.txt --ENCODE--> results/encoded.txt --DECODE--> file2.txt
```
You can compare the loss of information by running `diff file1.txt file2.txt`. There shouldn't be any difference (pending any bugs etc.). You can also see the size of `encoded.txt` to see the reduction of size. If you open it, you will see that it's just a pile of apparently nonsensical characters. 

## Disclaimer
This code is for recreational purposes so by no means should you use it for any sensitive purpose. There is no garantee against data loss or any other such issue.
