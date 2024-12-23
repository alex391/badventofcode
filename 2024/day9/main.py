# Use pypy3
def checksum(disk: list[int]) -> int:
    sum = 0
    for position, block in enumerate(disk):
        if block > 0:
            sum += position * block
    return sum


def print_no_spaces(disk: list[int]):
    for block in disk:
        print(block if block >= 0 else ".", end="")
    print()


def is_fragmented(disk: list[int]) -> bool:
    # Does it encounter some positive ints, then just some negative ints, then end of list?
    disk_itt = (item for item in disk)
    while True:
        try:
            # This is kinda weird but we need next() to raise StopIteration
            while next(disk_itt) >= 0:
                pass
            while next(disk_itt) < 0:
                pass
            return False
        except StopIteration:
            return True


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
    right_index = len(disk) - 1
    left_index = 0
    while left_index < right_index:
        if is_fragmented(disk):
            break

        if disk[left_index] < 0:
            # swap: https://stackoverflow.com/a/2493962
            disk[left_index], disk[right_index] = disk[right_index], disk[left_index]
            right_index -= 1

        else:
            left_index += 1

    # print_no_spaces(disk)

    print(checksum(disk))


if __name__ == "__main__":
    main()
