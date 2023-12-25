class Hand():
    def __init__(self, colors):
        self.red = colors["red"] if "red" in colors else 0
        self.green = colors["green"] if "green" in colors else 0
        self.blue = colors["blue"] if "blue" in colors else 0

    def __repr__(self) -> str:
        return f"red: {self.red}, green: {self.green}, blue: {self.blue}"

    def power(self) -> int:
        return self.red * self.green * self.blue


def main():
    sum = 0
    with open("input.txt") as input:
        for id, line in enumerate(input):
            id += 1
            hands_part = line.split(":")[1].strip()
            hand_strings = [hand.strip() for hand in hands_part.split(";")]
            game = []
            for string in hand_strings:
                cubes = [cube.strip() for cube in string.split(",")]
                counts_and_colors = {color: int(count) for count, color in [
                    cube.split() for cube in cubes]}
                hand = Hand(counts_and_colors)
                game.append(hand)
            fewest_cubes = min_hand(game)
            sum += fewest_cubes.power()
    print(sum)


def min_hand(hands: list) -> Hand:
    return Hand({
        "red": max([hand.red for hand in hands]),
        "green": max([hand.green for hand in hands]),
        "blue": max([hand.blue for hand in hands])
    })


if __name__ == "__main__":
    main()
