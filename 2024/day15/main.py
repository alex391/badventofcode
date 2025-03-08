import copy


class Point:
    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y

    def __add__(self, other):
        return Point(
            self.x + other.x,
            self.y + other.y
        )
    def __iadd__(self, other):
        self.x += other.x
        self.y += other.y
        return self
        


def push(grid: list[list[str]], robot: Point, instruction: str):
    direction_lookup = {
        "^": Point(0, -1),
        ">": Point(1, 0),
        "v": Point(0, 1),
        "<": Point(-1, 0),
    }
    look_direction = direction_lookup[instruction]
    look_cursor = copy.deepcopy(robot)
    moved_box = False
    while True:
        look_cursor += look_direction
        under_cursor = grid[look_cursor.y][look_cursor.x]
        if under_cursor == "#":
            break
        if under_cursor == "O":
            moved_box = True
            continue
        grid[robot.y][robot.x] = "."
        if moved_box:
            grid[look_cursor.y][look_cursor.x] = "O"
        robot += look_direction
        grid[robot.y][robot.x] = "@" # Just for visualization
        break




def main():
    grid: list[list[str]] = []
    instructions: list[str] = []
    with open("input.txt", "r") as f:
        for line in f:
            if line.strip() == "":
                break
            row = [c for c in line.strip()]
            grid.append(row)
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
