class Point:
    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y


def push(grid: list[list[str]], robot: Point, instruction: str):
    direction_lookup = {
        "^": Point(0, 1),
        ">": Point(1, 0),
        "v": Point(0, -1),
        "<": Point(-1, 0),
    }
    look_direction = direction_lookup[instruction]


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


if __name__ == "__main__":
    main()
