import copy
from typing import Optional


class Point:
    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y

    # https://stackoverflow.com/a/2909119
    def __key(self):
        return (self.x, self.y) 
    
    def __hash__(self):
        return hash(self.__key())
    
    def __eq__(self, other):
        return self.x == other.x and self.y == other.y


    def __add__(self, other):
        return Point(self.x + other.x, self.y + other.y)

    def __iadd__(self, other):
        self.x += other.x
        self.y += other.y
        return self


def adjacent_boxes(
    grid: list[list[str]], box: Point, direction: Point
) -> Optional[set[Point]]:
    """
    return a set of Points, representing coordinates of connected boxes, or
    None if a # is encountered
    """
    adjacent = set()
    box_type = grid[box.y][box.x]
    if box_type == "[":
        adjacent.add(box + Point(1, 0))
    else:
        adjacent.add(box + Point(-1, 0))
    neighbor_coordinate = box + direction
    neighbor = grid[neighbor_coordinate.y][neighbor_coordinate.x]
    if neighbor == "[" or neighbor == "]":
        adjacent.add(neighbor_coordinate)
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


    if grid[robot.y + direction.y][robot.x + direction.x] == ".":
        # Nothing to do except move the robot
        grid[robot.y][robot.x] = "."
        robot += direction
        grid[robot.y][robot.x] = "@"
        return
    if grid[robot.y + direction.y][robot.x + direction.x] == "#":
        return

    s = []
    discovered = set()
    s.append(robot + direction)
    while len(s) > 0:
        v = s.pop()
        if v not in discovered:
            discovered.add(v)
            edges = adjacent_boxes(grid, v, direction)
            if edges is None:
                return # Just do nothing if we can't move the whole group
            for w in edges:
                s.append(w)
    
    temp_grid = copy.deepcopy(grid)
    for box_half in discovered:
        grid[box_half.y][box_half.x] = "."

    grid[robot.y][robot.x] = "."
    
    for box_half in discovered:
        moved = box_half + direction
        grid[moved.y][moved.x] = temp_grid[box_half.y][box_half.x]

    robot += direction
    grid[robot.y][robot.x] = "@"


DIRECTION_LOOKUP = {
    "^": Point(0, -1),
    ">": Point(1, 0),
    "v": Point(0, 1),
    "<": Point(-1, 0),
}

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
                    continue
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
        push_connected_boxes(grid, robot, DIRECTION_LOOKUP[instruction])

    # GPS
    gps = 0
    for y, row in enumerate(grid):
        for x, column in enumerate(row):
            if column == "[":
                gps += (100 * y) + x
    print(gps)


if __name__ == "__main__":
    main()
