



import os

file_prefix = "./CSVD/"

def main():
    w = os.listdir(file_prefix)
    q = w
    q = [k.replace(".NS", "")
    for k, v in enumerate(w):
        os.rename(prefix+v, prefix+q[k])
    print("renaming completed!")


if __name__ == "__main__":
    main()
