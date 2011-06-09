CXX=g++
CC=gcc
LD=ld
CXX_SRC=$(wildcard src/*.cc)
C_SRC=$(wildcard lib/lzma/*.c)
OBJ=$(CXX_SRC:.cc=.o) $(C_SRC:.c=.o)
LDFLAGS=-lz
CFLAGS=-Wall -g -L. -arch i386 -arch x86_64
INCLUDE=-Iinclude -Ilib/lzma/
DEFINES=-D_7ZIP_ST
LIBVER=1.0
DYLIB=libawd.dylib
STATLIB=libawd.a
PREFIX=/usr/local
LIBDIR=$(PREFIX)/lib
INCDIR=$(PREFIX)/include
BUILDDIR=build

all: dynamic

test: 
	$(CXX) -dynamic -lawd -o test test.cc
    

install: dynamic 
	cp $(BUILDDIR)/$(DYLIB) $(LIBDIR)/$(DYLIB)
	rm -rf $(INCDIR)/awd
	cp -Rf $(BUILDDIR)/awd $(INCDIR)/
    

dynamic: $(OBJ) 
	$(CXX) -dynamiclib -install_name $(LIBDIR)/$(DYLIB) -current_version $(LIBVER) $(LDFLAGS) $(CFLAGS) $(OBJ) -o $(BUILDDIR)/$(DYLIB)
	mkdir -p $(BUILDDIR)/awd/
	cp include/*.h $(BUILDDIR)/awd/

static: $(OBJ) 
	ar crs $(STATLIB) $(OBJ)

.c.o:
	$(CC) -c $(INCLUDE) $(CFLAGS) $(DEFINES) $< -o $@

.cc.o:
	$(CXX) -c $(INCLUDE) $(CFLAGS) $(DEFINES) $< -o $@
	
clean: clean-objects
	rm -rf $(BUILDDIR)/*

clean-objects:
	rm -f $(OBJ)
