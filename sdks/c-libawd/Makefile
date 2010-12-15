CC=gcc
LD=ld
SRC=$(wildcard src/*.c lib/lzma/*.c)
#OBJ=src/awd.o src/util.o src/writing.o src/attr.o
OBJ=$(SRC:.c=.o)
CFLAGS=-Wall -L. -arch i386 -arch ppc -arch x86_64
INCLUDE=-Iinclude -Ilib/lzma/
DEFINES=-D_7ZIP_ST
LIBVER=1.0
DYLIB=libawd.dylib
STATLIB=libawd.a
PREFIX=/usr/local
LIBDIR=$(PREFIX)/lib
INCDIR=$(PREFIX)/include
BUILDDIR=build

all: dynamic awdtest

install: dynamic 
	cp $(BUILDDIR)/$(DYLIB) $(LIBDIR)/$(DYLIB)
	cp -R $(BUILDDIR)/awd $(INCDIR)/awd
    

awdtest: dynamic 
	$(CC) -lawd $(INCLUDE) $(CFLAGS) test.c -o awdtest

dynamic: $(OBJ) 
	gcc -dynamiclib -install_name $(LIBDIR)/$(DYLIB) -current_version $(LIBVER) $(CFLAGS) $(OBJ) -o $(BUILDDIR)/$(DYLIB)
	mkdir -p $(BUILDDIR)/awd/
	cp include/*.h $(BUILDDIR)/awd/

static: $(OBJ) 
	ar crs $(STATLIB) $(OBJ)

.c.o:
	$(CC) -c $(INCLUDE) $(CFLAGS) $(DEFINES) $< -o $@
	
clean: clean-objects
	rm -f awdtest

clean-objects:
	rm -f *.o
	rm -rf $(BUILDDIR)/*
	rm -f $(OBJ)
