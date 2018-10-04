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

    with ctx.create_virtualenv(cwd=ctx.bldnode.abspath()) as venv:
        if not ctx.options.all_docs:
            venv.run('python -m pip install -r docs/requirements.txt',
                     cwd=ctx.path.abspath())
            venv.run('sphinx-build -b html -d build/doctrees docs build/html',
                     cwd=ctx.path.abspath())
        else:
            giit = 'git+https://github.com/steinwurf/giit.git@cffca2e4'
            venv.pip_install(packages=[giit])
            venv.run('giit clean .', cwd=ctx.path.abspath())
            venv.run('giit sphinx .', cwd=ctx.path.abspath())
            venv.run('giit landing_page .', cwd=ctx.path.abspath())
            #if ctx.options.publish_docs:
            #    venv.run('giit gh_pages .', cwd=ctx.path.abspath())

