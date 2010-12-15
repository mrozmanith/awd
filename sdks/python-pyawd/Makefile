BLENPATH=/Applications/blender-2.54.app/Contents/MacOS/2.54/python/lib/python3.1
LIBPATH=./build/lib.macosx-10.3-fat-3.1

DIST_CFLAGS=-arch ppc -arch i386 -arch x86_64 -isysroot /Developer/SDKs/MacOSX10.4u.sdk -fno-strict-aliasing -fno-common -dynamic -I/usr/local/include/

SO=pyawd.so
SITE=$(BLENPATH)/site-packages


install: dist_install blender_install

dist_install:
	CFLAGS="$(DIST_CFLAGS)" python3 setup.py clean build install

blender_install:
	cp $(LIBPATH)/$(SO) $(SITE)/$(SO)

clean:
	rm -rf build
