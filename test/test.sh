#!/bin/bash

# Change this line to be able to call it from elswhere
#e.g. TESTS_DIR=~/Code/HuffmanCoding/test
TESTS_DIR=$PWD

# Dependencies
TESTFILE="$TESTS_DIR/testdata/humanrights.txt"
ENCODED="$TESTS_DIR/testdata/humanrights.huf"
NEWENCODED="$TESTS_DIR/humanrights.huf"
DECODED="$TESTS_DIR/humanrights.txt"

# Running program
cd ..
./huffcode -e "$TESTFILE"
mv "$ENCODED" "$NEWENCODED"
./huffcode -d "$NEWENCODED"

# Showing diffs
echo
echo "Difference between original and encoded-decoded (blank if none, ok):"
diff "$TESTFILE" "$DECODED"
echo

# Showing compression
OWEIGHT=$(wc -c "$TESTFILE" | awk '{print $1}')
CWEIGHT=$(wc -c "$NEWENCODED" | awk '{print $1}')

echo "The original weights:   $OWEIGHT bytes"
echo "The compressed weights:  $CWEIGHT bytes"

echo "This represents a $(( (( (($OWEIGHT-$CWEIGHT)) * 100))/$OWEIGHT))% reduction"

# Removing test files
echo
echo "Results files removed"
rm -f "$NEWENCODED"
rm -f "$DECODED"