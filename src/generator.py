#!/usr/bin/env python3

import sys
import random

if len(sys.argv) != 3:
    print("Usage: python3 generate.py <n> <filename>", file=sys.stderr)
    sys.exit(1)

n = int(sys.argv[1])
filename = sys.argv[2]

with open(filename, 'w') as f:
    for _ in range(n):
        id_value = random.randint(0, 999999)
        value = random.randint(0, 2**64 - 1)

        f.write(f"{id_value:06d}\t{value}\n")

print(f"Generated {n} lines in {filename}")
