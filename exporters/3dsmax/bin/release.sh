#!/bin/sh

# This script can be used to package the builds into ZIP files ready for
# release. It should be executed using a bash compatible shell from this 
# directory. The zip utility needs to be available (default on Mac OSX.)

mkdir -p releases;
for f in *.dle; do
    release=${f%.*};
    zip -r releases/$release"_v"$1.zip README.txt maxawd $f;
done;

