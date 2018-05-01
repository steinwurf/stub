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
    with _create_virtualenv(bld=ctx) as venv:

        venv.env['PATH'] = os.path.pathsep.join(
            [venv.env['PATH'], os.environ['PATH']])

        venv.pip_install(packages=['sphinx', 'wurfdocs'])

        # venv.run('sphinx-versioning build -r add-docs docs docs/_build/html',
        #          cwd=ctx.path.abspath())

        git = Git(git_binary='git', ctx=ctx)

        cwd = ctx.path.abspath()
        build_dir = tempfile.gettempdir()

        current_branch, other_branches = git.branch(cwd=cwd)
        tags = git.tags(cwd=cwd)

        checkouts = [current_branch] + other_branches + tags
        checkouts = [c.strip() for c in checkouts]

        print(checkouts)

        for checkout in checkouts:

            checkout_path = os.path.join(build_dir, 'checkouts', checkout)

            if os.path.isdir(checkout_path):
                remove_directory(path=checkout_path)

            shutil.copytree(src=cwd, dst=checkout_path, symlinks=True)

            args = ['git', 'checkout', '-f', checkout]
            ctx.cmd_and_log(args, cwd=checkout_path)

            docs_path = os.path.join(build_dir, 'docs', checkout)

            try:

                venv.run('sphinx-build --no-color -w log.txt -b html docs {}'.format(
                    docs_path), cwd=checkout_path)

            except Exception:
                continue

        print("Current branch {}".format(
            git.current_branch(cwd=ctx.path.abspath())))


def _create_virtualenv(bld):

    # Create a virtualenv in the source folder and build universal wheel
        # Make sure the virtualenv Python module is in path
    venv_path = bld.dependency_path('virtualenv')

    env = dict(os.environ)
    env.update({'PYTHONPATH': os.path.pathsep.join(
        [bld.dependency_path('virtualenv')])})

    return bld.create_virtualenv(cwd=bld.bldnode.abspath(), env=env)
