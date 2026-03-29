#!/usr/bin/env python3

import sys


def main():
    infile = sys.argv[1]

    fd = open(infile, "r")
    lines = fd.readlines()
    fd.close()
    for line in lines:
        ln = line.strip()
        print('{"' + ln + '",', len(ln), "},")


if __name__ == "__main__":
    main()
