import staticjinja

if __name__ == "__main__":
    site = staticjinja.make_site(env_globals={})

    site.render()
