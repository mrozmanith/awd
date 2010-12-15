#!/usr/local/bin/python3

from distutils.core import setup, Extension

source_files = [
    'src/pyawd.c',
    'src/util.c',
    'src/AWD.c',
    'src/AWDMeshData.c',
    'src/AWDMeshInst.c',
    'src/AWDSubMesh.c'
]

awd = Extension('pyawd', 
    libraries = [ 'awd' ],
    sources = source_files )

setup(name = 'pyawd',
    version = '0.1.0',
    description = 'Python package for dealing with Away3D AWD files',
    ext_modules = [ awd ])

