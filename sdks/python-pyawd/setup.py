#!/usr/bin/env python

import os
import sys
from distutils.core import setup, Extension

version_macro = ('PYTHON_VERSION', sys.version_info[0])

source_files = [
    'src/util.cc',
    'src/io.cc',
    'src/AWDWriter.cc'
]

libawd_io = Extension('pyawd.io', 
    libraries = [ 'awd' ],
    include_dirs = [ 'include', '/usr/local/include' ],
    define_macros = [ version_macro ],
    sources = source_files )

setup(name = 'pyawd',
    version = '0.1.0',
    description = 'Python package for dealing with Away3D AWD files',
    ext_modules = [ libawd_io ],
    package_dir = { 'pyawd': 'src/pyawd' },
    packages = [ 'pyawd' ]
    )

