# -- General configuration ------------------------------------------------

# If your documentation needs a minimal Sphinx version, state it here.
#
# needs_sphinx = '1.0'

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = ['wurfapi']

# Wurfdocs options - relative to your docs dir
wurfapi = {
    'source_paths': [
        '../src/stub/function.hpp',
        '../src/stub/return_handler.hpp',
    ],
    'recursive': False,
    'include_paths': ['../src'],
    'parser': {
        'type': 'doxygen',
        'download': True,
        'warnings_as_error': False
    }
}

# Add any paths that contain templates here, relative to this directory.
templates_path = []

# The suffix(es) of source filenames.
# You can specify multiple suffix as a list of string:
#
# source_suffix = ['.rst', '.md']
source_suffix = '.rst'

# The master toctree document.
master_doc = 'index'

# General information about the project.
project = u'Stub'
copyright = u'2018, contact@steinwurf.com'
author = u'contact@steinwurf.com'

# The version info for the project you're documenting, acts as replacement for
# |version| and |release|, also used in various other places throughout the
# built documents.
#
# The short X.Y version.
version = u''
# The full version, including alpha/beta/rc tags.
release = u''

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
#
# This is also used if you do content translation via gettext catalogs.
# Usually you set "language" from the command line for these cases.
language = None

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This patterns also effect to html_static_path and html_extra_path
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = 'sphinx'

# If true, `todo` and `todoList` produce output, else they produce nothing.
todo_include_todos = False


# -- Options for HTML output ----------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = "guzzle_sphinx_theme"


# Theme options are theme-specific and customize the look and feel of a theme
# further.  For a list of options available for each theme, see the
# documentation.
#
# html_theme_options = {}

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
#html_static_path = ['_static']


if html_theme == 'guzzle_sphinx_theme':

    import guzzle_sphinx_theme

    html_theme_path = guzzle_sphinx_theme.html_theme_path()

    # Register the theme as an extension to generate a sitemap.xml
    extensions.append("guzzle_sphinx_theme")

    # Guzzle theme options (see theme.conf for more information)
    html_theme_options = {
        # Set the name of the project to appear in the sidebar
        "project_nav_name": "Stub",
        "version_json_location": "./versjon.json"
    }

    html_sidebars = {
        '**': ['logo-text.html',
               'globaltoc.html', 'searchbox.html',
               'versions.html']
    }
