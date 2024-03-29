#! /usr/bin/env python
# encoding: utf-8

import os

from waflib.Build import BuildContext

APPNAME = "stub"
VERSION = "7.1.1"


def configure(conf):
    conf.set_cxx_std(11)


def build(bld):
    bld(name="stub_includes", includes="./src", export_includes="./src")

    if bld.is_toplevel():
        # Only build tests when executed from the top-level wscript,
        # i.e. not when included as a dependency
        bld.recurse("test")


def docs(ctx):
    """Build the documentation in a virtualenv"""

    with ctx.create_virtualenv() as venv:
        # To update the requirements.txt just delete it - a fresh one
        # will be generated from test/requirements.in
        if not os.path.isfile("docs/requirements.txt"):
            venv.run("python -m pip install pip-tools")
            venv.run("pip-compile docs/requirements.in")

        venv.run("python -m pip install -r docs/requirements.txt")

        venv.run("giit clean . --build_path build/site/docs", cwd=ctx.path.abspath())
        venv.run("giit sphinx . --build_path build/site/docs", cwd=ctx.path.abspath())
