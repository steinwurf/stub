#! /usr/bin/env python
# encoding: utf-8

APPNAME = 'stub'
VERSION = '5.0.1'


def build(bld):

    bld.env.append_unique(
        'DEFINES_STEINWURF_VERSION',
        'STEINWURF_STUB_VERSION="{}"'.format(VERSION))

    bld(name='stub_includes',
        includes='./src',
        export_includes='./src')

    if bld.is_toplevel():

        # Only build tests when executed from the top-level wscript,
        # i.e. not when included as a dependency
        bld.recurse('test')
