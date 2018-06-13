import staticjinja
import click
import tempfile
import os


@click.command()
@click.option('--outpath')
@click.option('--docspath')
def build(outpath, docspath):
    data = {
        'name': 'stub'
    }

    if not outpath:
        tempdir = tempfile.gettempdir()
        outpath = os.path.join(tempdir, 'stub-landing')

        if not os.path.isdir(outpath):
            os.makedirs(outpath)

    versions = []

    if docspath:

        for name in os.listdir(docspath):

            dirpath = os.path.join(docspath, name)

            if not os.path.isdir(dirpath):
                continue

            path = os.path.relpath(path=dirpath, start=outpath)

            versions.append((name, path))

    data['versions'] = versions

    print(versions)

    site = staticjinja.make_site(
        contexts=[('index.html', data)],
        staticpaths=['css', 'js', 'images'],
        outpath=outpath)

    site.render()


if __name__ == "__main__":
    build()
