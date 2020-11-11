# Huffman Coding

Implementing Huffman coding as a recreation project. Still in early stage. 

## Progress report
So far the program:
- Reads a file and counts the frequency of each ASCII character
- Builds a huffmann tree
- Encodes the text in file 1 and stores it
- Reads the encoded file and reverts to the original text

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
As you can see the last character is missing (a dot/full stop). However that is the only error, so the results seem promissing. This text contains 10850 characters or bytes, and gets reduced to 8377 bites, a 77.2% of the original. This could be further improved by excluding characters with zero frequency from the huffmann tree, or applying other natural text compression alorithms beforehand.


## How to use
First clone the repository and, on linux, run `sh build.sh`. On windows you're on your own but it is a small project so it shouldn't be difficult.

A symbolic link will be created in the repository directory, as well as a results directory. You can run a command such as: `huffman file1.txt file2.txt`. The first file will be your input file. The second one is the output. There will also appear a  `results/encoded.txt`, with the encoded file. The program will perform the following process:
```
file1.txt --ENCODE--> results/encoded.txt --DECODE--> file2.txt
```
You can compare the loss of information by running `diff file1.txt file2.txt`. There shouldn't be any difference (pending any bugs etc.). You can also see the size of `encoded.txt` to see the reduction of size. If you open it, you will see that it's just a pile of apparently nonsensical characters. 

## Disclaimer
This code is for recreational purposes so by no means should you use it for any sensitive purpose. There is no garantee against data loss or any other such issue.
