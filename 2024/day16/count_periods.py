def main():
    count = 0
    with open("input.txt") as f:
        for line in f:
            for c in line:
                if c != '#':
                    count += 1
        print(count)


if __name__ == "__main__":
    main()
