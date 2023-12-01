def main():
    sum = 0
    with open("input.txt") as input:
        for line in input:
            first = None
            last = None
            for char in line:
                value = 0
                try:
                    value = int(char)
                except ValueError:
                    continue # Just skip it
                if (first == None):
                    first = value
                else:
                    last = value
            if (last == None):
                last = first
            sum += (first * 10) + last
                    
    print("part 1:", sum)
            


if __name__ == "__main__":
    main()