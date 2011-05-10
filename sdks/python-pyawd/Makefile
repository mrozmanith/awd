BLENPATH=/Applications/blender-2.54.app/Contents/MacOS/2.54/python/lib/python3.1
LIBPATH31=./build/lib.macosx-10.3-fat-3.1
LIBPATH26=./build/lib.macosx-10.4-x86_64-2.6

#DIST_CFLAGS=-g -arch ppc -arch i386 -arch x86_64 -isysroot /Developer/SDKs/MacOSX10.4u.sdk -fno-strict-aliasing -fno-common -dynamic -I/usr/local/include/
DIST_CFLAGS=-g -arch i386 -arch x86_64 -fno-strict-aliasing -fno-common -dynamic -I/usr/include
CFLAGS=-c -O0 $(DIST_FLAGS)

SO=pyawd.so
BLENSITE=$(BLENPATH)/site-packages
MAYASITE=/Applications/Autodesk/maya2011/Maya.app/Contents/Frameworks/Python.framework/Versions/2.6/lib/python2.6/site-packages


install: dist_install26 dist_install31 blender_install maya_install

build26:
	CFLAGS="$(DIST_CFLAGS)" python2.6 setup.py clean build

build31:
	CFLAGS="$(DIST_CFLAGS)" python3 setup.py clean build

dist_install26: build26
	python2.6 setup.py install
    
dist_install31: build31
	python3 setup.py install
    
blender_install: build31
	cp $(LIBPATH31)/$(SO) $(BLENSITE)/$(SO)

maya_install: build26
	cp $(LIBPATH26)/$(SO) $(MAYASITE)/$(SO)

clean:
	rm -rf build
