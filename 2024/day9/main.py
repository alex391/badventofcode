def main():
    disk: list[int] = []  # ID or -1 for blank
    id = 0
    is_free = True
    with open("input.txt", "r") as f:
        while True:
            try:
                if is_free:
                    for _ in range(int(f.read(1))):
                        disk.append(id)
                    id += 1
                else:
                    for _ in range(int(f.read(1))):
                        disk.append(-1)
                is_free = not is_free
            except ValueError:
                break

    print(disk)


if __name__ == "__main__":
    main()
