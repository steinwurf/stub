#! /usr/bin/env python
# encoding: utf-8

import os

from waflib.Build import BuildContext

APPNAME = "stub"
VERSION = "7.1.1"

def options(ctx):
    ctx.load("cmake")


def configure(ctx):

    ctx.load("cmake")

    if ctx.is_toplevel():
        ctx.cmake_configure()


def build(ctx):

    ctx.load("cmake")

    if ctx.is_toplevel():
        ctx.cmake_build()



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
