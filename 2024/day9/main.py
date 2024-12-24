# Use pypy3
from typing import NamedTuple


class File(NamedTuple):
    index: int
    length: int

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

def has_space_for(file: File, index: int, disk: list[int]) -> bool:
    for offset in range(file.length):
        if offset + index > file.index:
            return False
        if disk[offset + index] >= 0:
            return False
    return True


def main():
    disk: list[int] = []  # ID or -1 for blank
    file_table: list[File] = [] # table from ID to File
    id = 0
    is_free = True
    with open("input.txt", "r") as f:
        while True:
            try:
                if is_free:
                    file_start_index = len(disk)
                    file_length = int(f.read(1))
                    for _ in range(file_length):
                        disk.append(id)
                    file_table.append(File(file_start_index, file_length))
                    id += 1
                else:
                    for _ in range(int(f.read(1))):
                        disk.append(-1)
                is_free = not is_free
            except ValueError:
                break
    # print_no_spaces(disk)
    left_index = 0
    while len(file_table) > 0:
        id = len(file_table) - 1
        file = file_table.pop()
        while True:
            try:
                has_space = has_space_for(file, left_index, disk)
            except IndexError:
                left_index = 0
                break
            if has_space:
                for i in range(file.length):
                    disk[left_index + i], disk[file.index + i] = disk[file.index + i], -1
                # print_no_spaces(disk)
                left_index = 0
                break # We found space for this file!
            else:
                left_index += 1
                if left_index >= len(disk):
                    left_index = 0
                    break





    # print_no_spaces(disk)

    print(checksum(disk))


if __name__ == "__main__":
    main()
