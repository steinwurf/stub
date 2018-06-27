#! /usr/bin/env python
# encoding: utf-8

import os
import tempfile
import shutil

from waflib.Build import BuildContext
from waflib.extras.wurf.git import Git
from waflib.extras.wurf.directory import remove_directory

APPNAME = 'stub'
VERSION = '6.2.0'


class DocsContext(BuildContext):
    cmd = 'docs'
    fun = 'docs'


def options(opt):
    opt.add_option(
        '--publish', default=False, action='store_true',
        help='Publish the documentation')


def resolve(ctx):

    # Testing dependencies
    ctx.add_dependency(
        name='virtualenv',
        recurse=False,
        optional=False,
        resolver='git',
        method='checkout',
        checkout='15.1.0',
        sources=['github.com/pypa/virtualenv.git'])


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
    with _create_virtualenv(bld=ctx) as venv:

        venv.env['PATH'] = os.path.pathsep.join(
            [venv.env['PATH'], os.environ['PATH']])

        venv.pip_install(packages=["giit"])

        venv.run('giit sphinx .',
                 cwd=ctx.path.abspath())

        venv.run('giit landing_page .',
                 cwd=ctx.path.abspath())

        if ctx.options.publish:

            venv.run('giit gh_pages .',
                     cwd=ctx.path.abspath())


def _create_virtualenv(bld):

    # Create a virtualenv in the source folder and build universal wheel
        # Make sure the virtualenv Python module is in path
    venv_path = bld.dependency_path('virtualenv')

    env = dict(os.environ)
    env.update({'PYTHONPATH': os.path.pathsep.join(
        [bld.dependency_path('virtualenv')])})

    return bld.create_virtualenv(cwd=bld.bldnode.abspath(), env=env)
