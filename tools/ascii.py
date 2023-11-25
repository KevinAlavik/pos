def print_ascii_art(ascii_art):
    for line in ascii_art.split("\n"):
        print(f"println(\"{line}\");")

if __name__ == "__main__":
    ascii_art = """
    
        /\
      _/./
   ,-'    `-:.,-'/
  > O )<)    _  (
   `-._  _.:' `-.\\
       `` \\;

    """
    print_ascii_art(ascii_art)
