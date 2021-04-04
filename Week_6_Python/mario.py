# mario function to print pattern
def mario(h):
    for i in range(1, h+1):
        for j in range(i, h):
            print(" ", end="")
        for k in range(h-i, h):
            print("#",  end="")
        print()


# infinite loop for restricting height
while True:
    height = input("Height: ")
    h = -1
    try:
        h = int(height)
    except:
        pass
    if 0 < h < 9:
        mario(h)
        break
