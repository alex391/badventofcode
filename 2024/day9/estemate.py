if __name__ == "__main__":
    bytes_needed = 0
    with open("input.txt", "r") as f:
        while True:
            file = f.read(1)
            try:
                bytes_needed += int(file)
            except ValueError:
                break

    print(bytes_needed)
