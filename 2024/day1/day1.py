def main():
    left = []
    right = []
    with open("input.txt", "r") as f:
        for line in f:
            slplit = line.split("   ")
            left.append(int(slplit[0]))
            right.append(int(slplit[1]))
    left.sort()
    right.sort()

    distance_sum = 0
    for left, right in zip(left, right):
        distance_sum += abs(left - right)

    print(distance_sum)
    
if __name__ == "__main__":
    main()
