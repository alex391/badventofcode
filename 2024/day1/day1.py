from collections import Counter


def part_one():
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

    print(f"Part 1: {distance_sum}")


def part_two():
    left = []
    right = []
    with open("input.txt", "r") as f:
        for line in f:
            slplit = line.split("   ")
            left.append(int(slplit[0]))
            right.append(int(slplit[1]))

    similarity_score = 0
    right_counter = Counter(right)

    for item in left:
        similarity_score += item * right_counter[item]

    print(f"Part 2: {similarity_score}")


if __name__ == "__main__":
    part_one()
    part_two()
