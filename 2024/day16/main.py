# OK, I was making it more complicated than it needed to be so I've started
# over. I also had a look at
# https://todd.ginsberg.com/post/advent-of-code/2024/day16/

# also oh boy this is slow... use pypy3
import copy
from enum import Enum
from queue import PriorityQueue

BIG = 2**63 - 1  # arbitrary large number


class Point:
    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y

    def __add__(self, other):
        return Point(self.x + other.x, self.y + other.y)

    def list_lookup(self, l):
        return l[self.y][self.x]

    def __repr__(self) -> str:
        return f"({self.x}, {self.y})"

    def __eq__(self, other) -> bool:
        return self.x == other.x and self.y == other.y

    def key(self) -> tuple[int, int]:
        return self.x, self.y

    def __hash__(self) -> int:
        return hash(self.key())


class Direction(Enum):
    NORTH = Point(0, -1)
    EAST = Point(1, 0)
    SOUTH = Point(0, 1)
    WEST = Point(-1, 0)

    def clockwise(self):
        return {
            self.NORTH: self.EAST,
            self.EAST: self.SOUTH,
            self.SOUTH: self.WEST,
            self.WEST: self.NORTH
        }[self]

    def counterclockwise(self):
        return {
            self.NORTH: self.WEST,
            self.WEST: self.SOUTH,
            self.SOUTH: self.EAST,
            self.EAST: self.NORTH
        }[self]


class Location:
    def __init__(self, positions: list[Point], direction: Direction):
        self.positions: list[Point] = positions
        self.direction: Direction = direction

    def position(self) -> Point:
        return self.positions[-1]

    def step(self):
        self.positions.append(self.position() + self.direction.value)

    def unstep(self):
        self.positions.pop()

    def key(self) -> tuple[Point, Direction]:
        return self.position(), self.direction

    def clockwise(self):
        return Location(copy.deepcopy(self.positions), self.direction.clockwise())

    def counterclockwise(self):
        return Location(copy.deepcopy(self.positions), self.direction.counterclockwise())


class ScoredLocation:
    def __init__(self, location: Location, score: int):
        self.location = location
        self.score = score

    def __eq__(self, other) -> bool:
        return self.score == other.score

    def __lt__(self, other) -> bool:
        return self.score < other.score

    def __iter__(self):
        return iter((self.location, self.score))

    def __repr__(self) -> str:
        return f"position: {self.location.position()}, direction: {self.location.direction}, score: {self.score}"


def find_character(maze: list[str], target: str) -> Point:
    for row in enumerate(maze):
        for item in enumerate(row[1]):
            if item[1] == target:
                return Point(item[0], row[0])
    raise ValueError(f"Could not find target: {target}")


def traverse_maze(maze: list[str], start: Point, end: Point) -> int:
    q = PriorityQueue()
    seen: dict[tuple[Point, Direction], int] = {}

    q.put(ScoredLocation(Location([start], Direction.EAST), 0))

    while not q.empty():
        popped = q.get()
        location: Location = popped.location
        if type(location) is not Location:
            raise TypeError
        cost: int = popped.score
        if type(cost) is not int:
            raise TypeError
        if location.position() == end:
            return cost
        elif seen.get(location.key(), BIG) > cost:
            seen[location.key()] = cost
            q.put(ScoredLocation(location.clockwise(), cost + 1000))
            q.put(ScoredLocation(location.counterclockwise(), cost + 1000))
            location.step()
            if location.position().list_lookup(maze) != "#":
                q.put(ScoredLocation(copy.deepcopy(location), cost + 1))
            else:
                location.unstep()

    raise RuntimeError


def main():
    maze = []
    with open("input.txt") as file:
        for line in file:
            maze.append(line)

    start = find_character(maze, "S")
    end = find_character(maze, "E")
    score = traverse_maze(maze, start, end)
    print(score)


if __name__ == "__main__":
    main()
