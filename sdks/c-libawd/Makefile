CC=gcc
LD=ld
SRC=$(wildcard src/*.c lib/lzma/*.c)
OBJ=$(SRC:.c=.o)
CFLAGS=-Wall -g -L. -arch i386 -arch ppc -arch x86_64
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
	$(CC) -dynamic -lawd -o test test.c
    
    

install: dynamic 
	cp $(BUILDDIR)/$(DYLIB) $(LIBDIR)/$(DYLIB)
	rm -rf $(INCDIR)/awd
	cp -Rf $(BUILDDIR)/awd $(INCDIR)/
    

dynamic: $(OBJ) 
	gcc -dynamiclib -install_name $(LIBDIR)/$(DYLIB) -current_version $(LIBVER) $(CFLAGS) $(OBJ) -o $(BUILDDIR)/$(DYLIB)
	mkdir -p $(BUILDDIR)/awd/
	cp include/*.h $(BUILDDIR)/awd/

static: $(OBJ) 
	ar crs $(STATLIB) $(OBJ)

.c.o:
	$(CC) -c $(INCLUDE) $(CFLAGS) $(DEFINES) $< -o $@
	
clean: clean-objects
	rm -rf $(BUILDDIR)/*

clean-objects:
	rm -f $(OBJ)
