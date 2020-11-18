# Huffman Coding

Implementation of Huffman coding as a recreational project. Huffman coding is well explained in the [wikipedia page](https://en.wikipedia.org/wiki/Huffman_coding), but as a quick explanation:
- It is a lossless text compression algorithm.
- Works by creating an unbalanced binary tree with all characters at the leafs.
- Each characters' code is its adress in the tree.
- More frequent characters are higher up in the tree, therefore have shorter codes.

In ASCII each letter takes 8 bits. Once encoded, the most frequent characters take 4 bits while the least ones take 27. Note that most of the latter will be control characters that seldom, if ever, appear. This amounts can be reduced if we only build the tree with the subset of characters present in the file.

## Features
The program can:
- Encode a text and store it together with the tree.
- Read the encoded file and recover the original text.
- Store tree and data separately, and recover it later.

## Results
Here is the result of running the test:
```
Successfully encoded HuffmanCoding/test/testdata/humanrights.txt
Successfully decoded HuffmanCoding/test/humanrights.huf

Difference between original and encoded-decoded (blank if none, ok):

The original weights:   11253 bytes
The compressed weights:  6264 bytes
This represents a 44% reduction

Results files removed
```
We can see how there is no difference between the original and the reconstructed version. We also see that the compressed version wheights a 56% of the original for this case in particular.

## How to use
First clone the repository and, on linux, run `sh build.sh`. On windows you're on your own but it is a small project so it shouldn't be difficult.

A symbolic link will be created in the repository directory. You can run a command: `huffman -h` to see how to use it. It is quite simple. Once you get it running, you can compare the loss of information by running `diff original.txt decoded.txt`. There shouldn't be any difference. You can also see the size of `encoded.huf` to see the reduction in size. If you open it, you will see that it's just a pile of apparently nonsensical characters.

If you want to test the build, you can run tests yourself, by doing:
```
~/HuffmanCoding$ cd test
~/HuffmanCoding/test$ sh test.sh 
```
You can run different files by putting them in the `test/testdata` directory and modifying the first few lines of `test.sh`.

## How is data stored ?

### The pattern for the tree
To store the tree we store the information in differently-sized groups of bits. These are all appended one after the other without caring about where byte division is, in a continuous stream of bits. The pattern for the tree is:

- Non-leafs take 3 bits: `[false | has_left_child | has_right_child ]`
- Leafs take 9 bits: `[true | ASCII 0-7]`

The nodes are written always in the same order, in a recursive pattern starting from the root:
1. Write node info
2. If there is a Left child, start recursive pattern with it.
3. If there is a Right child, start recursive pattern with it.

When decoding, we know the tree has been finished when the previous three steps have been completed for the root node. We therefore do not need a separator character afterwards.

### The pattern for the data
The data is written also as a stream of bits:
- Each character is replaced by its adress in the tree.
- At the end of the text a EOT character (ASCII 0x04) is encoded and appended. For this reason, this character is disallowed in the rest of the text.
- Our encoded text is unlikely to have a number of bits multiple of 8, so trailing zeros are appended. The EOT character is needed so as not to interpret these trailing bits as data.

This is preformed twice, for the filename and for the data. Therefore the resulting binary contains

`<tree> <original filename> \EOT <contents> \EOT <trailing bits>`

## Potential improvements
Some possible features would be:
- Encoding/decoding multiple files with the same tree.
- Building a tree based on a snippet of text instead of the whole file.
- Restricting the character set to that of the file so as to make the tree smaller and therefore more efficient.

## Disclaimer
This code is for recreational purposes so by no means should you use it for any sensitive purpose. There is no garantee against data loss or any other such issue.
