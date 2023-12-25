import math


class Part():
    def __init__(self, part_number: int, coordinate: (int, int)):
        self.part_number = part_number
        self.coordinate = coordinate

    def __hash__(self) -> int:  # only unique if the coordinate is the same
        return hash(self.coordinate)

    def __eq__(self, other) -> bool:
        return self.coordinate == other.coordinate

    def __str__(self) -> str:
        return (
            f"({self.part_number}, "
            f"({self.coordinate[0]}, {self.coordinate[1]}))"
        )

    def __repr__(self) -> str:
        return self.__str__()


class Gear():
    def __init__(self, coordinate: (int, int), ratio: int):
        self.coordinate = coordinate
        self.ratio = ratio

    def __hash__(self) -> int:
        return hash(self.coordinate)

    def __eq__(self, other):
        return self.coordinate == other.coordinate


def main():
    engine = []
    with open("input.txt") as file:
        for line in file:
            engine.append(list(line.strip()))

    gears = set()
    for y, row in enumerate(engine):
        for x, column in enumerate(row):
            if column == '*':
                # Then it's a gear
                # check if there are numbers around it
                parts = set(check_for_numbers(engine, x, y))
                if len(parts) == 2:
                    product = 1
                    for part in parts:
                        product *= part.part_number
                    gears.add(Gear((x, y), product))

    # visualization = engine
    # for part in parts:
        # for i in range(number_length(part.part_number)):
        #     visualization[part.coordinate[1]][part.coordinate[0] + i] = "X"

    sum = 0
    for gear in gears:
        sum += gear.ratio

    # for y in visualization:
    #     for x in y:
    #         print(x, end="")
    # print()
    print(sum)


def check_for_numbers(engine: list, x: int, y: int) -> list:
    """Return a list of the numbers around it"""
    numbers_around_symbol = []
    for dy in range(-1, 2):
        for dx in range(-1, 2):
            try:
                if dx == 0 and dy == 0:
                    continue
                if engine[y + dy][x + dx].isdecimal():
                    numbers_around_symbol.append(
                        parse_number_in_engine(engine, x + dx, y + dy))
            except IndexError:
                continue
    return numbers_around_symbol


def parse_number_in_engine(engine: list, x: int, y: int) -> Part:
    """
        Read an number from the engine at the coordinates (backwards or
        forwards)
        Also return the coordinate of the starting value
    """
    row = engine[y]
    start = x
    while row[start].isdecimal():
        start -= 1
    start += 1

    number_string = ""
    i = start
    while row[i].isdecimal():
        number_string += row[i]
        if i == len(row) - 1:
            break
        i += 1
    return Part(int(number_string), (start, y))


def number_length(n: int) -> int:
    # https://math.stackexchange.com/a/1771462 thanks
    # https://math.stackexchange.com/users/73561/antoine
    return int(math.log10(n) + 1)


if __name__ == "__main__":
    main()
