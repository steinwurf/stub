#! /usr/bin/env python
# encoding: utf-8

import os

from waflib.Build import BuildContext

APPNAME = 'stub'
VERSION = '6.2.0'


class DocsContext(BuildContext):
    cmd = 'docs'
    fun = 'docs'


def options(opt):

    group = opt.add_option_group("Documentation")

    group.add_option(
        '--all_docs', default=False, action='store_true',
        help='Build all the docs (branches tags) otherwise only the '
             'working tree is built.')

    group.add_option(
        '--publish_docs', default=False, action='store_true',
        help='Publish the documentation.')


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


def docs(ctx):
    """ Build and push the documentation see giit.json for details. """
    with ctx.create_virtualenv() as venv:

        giit = 'git+https://github.com/steinwurf/giit.git@cffca2e'

        venv.run('pip install {}'.format(giit))

        venv.run('giit clean .', cwd=ctx.path.abspath())

        if ctx.options.all_docs or ctx.options.publish_docs:

            venv.run('giit sphinx .', cwd=ctx.path.abspath())

        else:
            venv.run('giit local-sphinx .', cwd=ctx.path.abspath())

        if ctx.options.publish_docs:

            venv.run('giit gh_pages .', cwd=ctx.path.abspath())
