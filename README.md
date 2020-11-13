# Huffman Coding

Implementing Huffman coding as a recreational project. Still in early stages. Huffman coding is well explained in the [wikipedia page](https://en.wikipedia.org/wiki/Huffman_coding), but as a quick explanation:
- It is a lossless text compression algorithm.
- Works by creating an unbalanced binary tree with all characters at the leafs.
- Each characters' code is its adress in the tree.
- More frequent characters are higher up in the tree, therefore have shorter codes.

In ASCII each letter takes 8 bits. Once encoded, the most frequent characters take 3 bits while the least ones take 27. Note that most of the latter will be control characters that seldom, if ever, appear. The set of characters can be restricted to make it more efficient. The tree can be constructed on a per-file basis or use generic precompiled values, e.g. the optimal for the english language.

## Progress report
So far the program:
- Reads a file and counts the frequency of each ASCII character
- Builds a huffmann tree
- Encodes a text and stores it together with the tree
- Reads the encoded file and recovers the original text

The program doesn't yet:
- Encode multiple files with the same tree.
- Use a snippet of text to build the tree instead of the whole text.
- Allow for non-ASCII characters.

## Current bugs
For certain files a SegFault is thrown. Still investigating the source of the error.

## Results
I ran the Universal Declaration of Human Rights as a sample text. This text contains 10850 characters or bytes, and gets reduced to 6281 bites, a 57.9% of the original. If we want to store the tree with it (which we need to decode), size climbs up to 6668 bites, or a 61.5% reduction. This could be further improved by excluding characters with zero frequency from the huffmann tree, or applying other natural text compression alorithms beforehand.

## How to use
First clone the repository and, on linux, run `sh build.sh`. On windows you're on your own but it is a small project so it shouldn't be difficult.

A symbolic link will be created in the repository directory, as well as a results directory. You can run a command such as: `huffman file1.txt file2.txt`. The first file will be your input file. The second one is the output. There will also appear a  `results/encoded.txt`, with the encoded file. The program will perform the following process:
```
file1.txt --ENCODE--> results/encoded.txt --DECODE--> file2.txt
```
You can compare the loss of information by running `diff file1.txt file2.txt`. There shouldn't be any difference. You can also see the size of `encoded.txt` to see the reduction of size. If you open it, you will see that it's just a pile of apparently nonsensical characters.

## How is data encoded?
A huffman tree is generated. This is a binary tree with letters at the edges. 

### The pattern for the tree
To store the tree we store the information in differently-sized groups of bits. These are all appended one after the other without caring about where byte division fall, in a continuous stream of bits. The pattern for the tree is:

- Non-leafs take 3 bits: `[false | has_left_child | has_right_child ]`
- Leafs take 9 bits: `[true | ASCII 0-7]`

The nodes are written always in the same order, in a recursive pattern starting from the root:
1. Write node info
2. If there is a Left child, start recursive pattern with it.
3. If there is a Right child, start recursive pattern with it.

This is not ideal for compression (for instance, storing only leaf nodes and their adresses could be more efficient, but slower and more complex). An advantage is that this method will allow for restricted sets of characters. 

### The pattern for the data
Then, without the need of any separator, the data is written:
- Each character is replaced by its adress in the tree.
- At the end of the text a EOT character (ASCII 0x04) is encoded and appended. For this reason, this character is disallowed in the rest of the text.
- Our encoded text is unlikely to have a number of bits multiple of 8, so trailing zeros are appended. The EOT character is needed so as not to interpret these trailing bits as data.

## Disclaimer
This code is for recreational purposes so by no means should you use it for any sensitive purpose. There is no garantee against data loss or any other such issue.
