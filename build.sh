MAIN_DIR=$PWD
BUILD_DIR=$MAIN_DIR/build
SOURCE_DIR=$MAIN_DIR/src
EXECUTABLE=huffcode

mkdir $BUILD_DIR

cd $BUILD_DIR

# Building
cmake $SOURCE_DIR
cmake --build .

# Making a link in main directory
cd $MAIN_DIR
if test -f "$EXECUTABLE"; then
	rm "$EXECUTABLE"
fi
ln -s "$BUILD_DIR/$EXECUTABLE" .