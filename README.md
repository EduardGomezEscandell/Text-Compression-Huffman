# Huffman Coding

Implementation of Huffman coding as a recreational project. Huffman coding is well explained in the [wikipedia page](https://en.wikipedia.org/wiki/Huffman_coding), but as a quick explanation:
- It is a lossless text compression algorithm.
- Works by creating an unbalanced binary tree with all characters at the leafs.
- Each characters' code is its adress in the tree.
- More frequent characters are higher up in the tree, therefore have shorter codes.

In ASCII each letter takes 8 bits. Once encoded, the most frequent characters take 4 bits while the least ones take 27. Note that most of the latter will be control characters that seldom, if ever, appear. The set of characters can be restricted to make it more efficient. The tree can be constructed on a per-file basis or use generic precompiled values, e.g. the optimal for the english language.

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
- Store trees separately.

## Results
Here is the result of running the test:
```
~/HuffmanCoding$ cd test
~/HuffmanCoding/test$ sh test.sh 
Successfully encoded ~/HuffmanCoding/test/humanrights.txt
Successfully decoded ~/HuffmanCoding/test/humanrights_copy.huf

Difference between original and encoded-decoded (blank if none, ok):

The original weights:   12557 bytes
The compressed weights:  7274 bytes
This represents a 42% reduction
```
We can see how there is no difference between the original and the reconstructed version. We also see that the compressed version wheights a 58% of the original for this case in particular. Due to the overhead of storing the tree, efficiency improves for larger files.


## How to use
First clone the repository and, on linux, run `sh build.sh`. On windows you're on your own but it is a small project so it shouldn't be difficult.

A symbolic link will be created in the repository directory, as well as a results directory. You can run a command: `huffman -h` to see how to use it. It is quite simple. Once you get it running, you can compare the loss of information by running `diff original.txt decoded.txt`. There shouldn't be any difference. You can also see the size of `encoded.huf` to see the reduction in size. If you open it, you will see that it's just a pile of apparently nonsensical characters.

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
