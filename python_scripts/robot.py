import math
import random


def test():
    for i in range(0, 5):
        print(math.sqrt(random.randint(10 * i, 10 * (i + 1))))
