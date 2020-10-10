



import os
import sys

def main():
    if len(sys.argv) < 2:
        print("Provide file_name e.g. python3 monitor ./resources/dfw.csv")
        sys.exit(1)
    file_name = str(sys.argv[1])
    w = os.listdir("./CSVD/")
    q = []
    with open(file_name) as f:
        q.append(f.readlines())
    q = q[0]
    q = [k.replace("\n", ".csv") for k in q]
    d = sorted(list(set(q) - set(w)))
    d = [k.replace(".csv", "") for k in d]
    for k in d:
        print(k, end=" ")
    print("total {} instruments".format(len(d)))

if __name__ == "__main__":
    main()
