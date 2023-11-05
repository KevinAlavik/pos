def print_ascii_art(ascii_art):
    for line in ascii_art.split("\n"):
        print(f"println(\"{line}\");")

if __name__ == "__main__":
    ascii_art = """
     ____   ___  ____  
    |  _ \ / _ \/ ___| 
    | |_) | | | \___ \ 
    |  __/| |_| |___) |
    |_|    \___/|____/ 
    """
    print_ascii_art(ascii_art)
