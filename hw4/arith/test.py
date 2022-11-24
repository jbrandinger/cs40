with open("comp.txt", "r") as f:
    for line in f:
        for i in line:
            print(int(i))