import copy
from typing import Optional


class Point:
    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y

    def __add__(self, other):
        return Point(self.x + other.x, self.y + other.y)

    def __iadd__(self, other):
        self.x += other.x
        self.y += other.y
        return self


def adjacent_boxes(
    grid: list[list[str]], box: Point, direction: Point
) -> Optional[list[Point]]:
    """
    return a list of Points, representing coordinates of connected boxes, or
    None if a # is encountered
    """
    adjacent = []
    box_type = grid[box.y][box.x]
    if box_type == "[":
        adjacent.append(box + Point(0, 1))
    else:
        adjacent.append(box + Point(0, -1))
    neighbor_coordinate = box + direction
    neighbor = grid[neighbor_coordinate.y][neighbor_coordinate.x]
    if neighbor == "[" or neighbor == "]":
        adjacent.append(neighbor_coordinate)
    elif neighbor == "#":
        return None
    return adjacent


def push_connected_boxes(grid: list[list[str]], robot: Point, direction: Point):
    # An example state: the robot is going to go up:
    # #######
    # #...#.#
    # #..[].#
    # #.[]..#
    # #.@...#
    # #######
    # and it shouldn't in this case
    # depth first search:
    # - [ is always connected to ] and vice versa
    # - if direction is ^ or v, then the [ or ] above/below is connected
    # - in any direction, if a # is encountered then we can exit early and do
    #   nothing
    # then move all the connected boxes up one

    # https://en.wikipedia.org/wiki/Depth-first_search
    s = []
    discovered = {robot + direction}
    s.push(robot + direction)
    while s.len() > 0:
        v = s.pop()
        if v not in discovered:
            discovered.add(v)
            edges = adjacent_boxes(grid, v, direction)




def push(grid: list[list[str]], robot: Point, instruction: str):
    direction_lookup = {
        "^": Point(0, -1),
        ">": Point(1, 0),
        "v": Point(0, 1),
        "<": Point(-1, 0),
    }
    look_direction = direction_lookup[instruction]
    look_cursor = copy.deepcopy(robot)
    while True:
        look_cursor += look_direction
        under_cursor = grid[look_cursor.y][look_cursor.x]
        if under_cursor == "#":
            break
        if under_cursor == ".":
            grid[robot.y][robot.x] = "."
            grid[look_cursor.y][look_cursor.x] = "@"


def main():
    grid: list[list[str]] = []
    instructions: list[str] = []
    with open("input.txt", "r") as f:
        for line in f:
            if line.strip() == "":
                break
            row = (c for c in line.strip())
            doubled_row = []
            for item in row:
                if item == "O":
                    doubled_row.append("[")
                    doubled_row.append("]")
                    continue
                if item == "@":
                    doubled_row.append("@")
                    doubled_row.append(".")
                doubled_row.append(item)
                doubled_row.append(item)
            grid.append(doubled_row)
        for line in f:
            instructions.extend(line.strip())

    robot = Point(0, 0)
    # Find where the robot is:
    for y, row in enumerate(grid):
        for x, column in enumerate(row):
            if column == "@":
                robot.x = x
                robot.y = y
                break

    # Then move:
    for instruction in instructions:
        push(grid, robot, instruction)

    # Visualization:
    for row in grid:
        for item in row:
            print(item, end="")
        print()

    # GPS
    gps = 0
    for y, row in enumerate(grid):
        for x, column in enumerate(row):
            if column == "O":
                gps += (100 * y) + x
    print(gps)


if __name__ == "__main__":
    main()
