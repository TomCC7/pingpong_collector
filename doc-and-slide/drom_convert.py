from wavedrom import render_file
from sys import argv


def main(arguments: list[str]):
    if len(arguments) <= 2:
        raise RuntimeError("Invalid Usage!")
    render_file(arguments[1], arguments[2])


if __name__ == "__main__":
    main(argv)
