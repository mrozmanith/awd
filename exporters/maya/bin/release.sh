#!/bin/sh

# This script can be used to package the builds into a ZIP file ready for
# release. It should be executed using a bash compatible shell from this 
# directory. The zip utility needs to be available (default on Mac OSX.)

PYAWD=../../../sdks/python-pyawd/build/lib.macosx-10.6-intel-2.7/pyawd
OUTPUT=releases/mayaawd-osx-$1.zip

rm -rf plug-ins/pyawd;
rm -f $OUTPUT

mkdir -p releases;
cp -R $PYAWD plug-ins/pyawd;
zip -r $OUTPUT README.txt plug-ins scripts;
