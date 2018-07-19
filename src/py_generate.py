from collections import Counter
from time import time

def pred(s):
    ca = Counter({str(ind): int(d) for ind, d in enumerate(str(s))})
    cb = Counter(str(s))
    return ca - cb == cb - ca == Counter()

top = 10 ** 8
start = time()

for i in range(1, top):
    if i % 10000 == 0:
        print("\r{:6.1%} ({:7.1e}/{:7.1e} @ {:7.1e} guess/s".format(i / top, i, top, i / (time() - start)), end="")
    if pred(i):
        print("\r{}\r".format(" " * 80), end="")
        print("{}".format(i))
