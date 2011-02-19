#!/usr/local/bin/python3

import os
import sys
from distutils.core import setup, Extension

version_macro = ('PYTHON_VERSION', sys.version_info[0])

source_files = [
    'src/pyawd.cc',
    'src/util.cc',
    'src/AWD.cc',
    'src/AWDAttrBlock.cc',
    'src/AWDSkeleton.cc',
    'src/AWDSkeletonJoint.cc',
    'src/AWDMeshData.cc',
    'src/AWDMeshInst.cc',
    'src/AWDSubMesh.cc'
]

pyawd = Extension('pyawd', 
    libraries = [ 'awd' ],
    include_dirs = [ '/usr/local/include' ],
    define_macros = [ version_macro ],
    sources = source_files )

setup(name = 'pyawd',
    version = '0.1.0',
    description = 'Python package for dealing with Away3D AWD files',
    ext_modules = [ pyawd ])

