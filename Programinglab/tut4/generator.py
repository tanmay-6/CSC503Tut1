import random

with open("random.txt","w") as f:
    for _ in range(1000000):
        n = random.randint(0,100)
        f.write(f"{n}\n")