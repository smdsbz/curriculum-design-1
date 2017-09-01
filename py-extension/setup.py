#!/usr/bin/env python3

from distutils.core import setup, Extension

module = Extension('curr',
                   sources=['curr.c',
                            '../utils/io_functions.c',
                            '../utils/faculty_functions.c',
                            '../utils/team_functions.c',
                            '../utils/project_functions.c'
                            # '../main.c'   # cause "multiple definition"
                           ])

setup(name='curr',
      ext_modules=[module])
