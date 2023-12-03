class Hand():
    MAX_RED = 12
    MAX_GREEN = 13
    MAX_BLUE = 14
    def __init__(self, colors):
        self.red = colors["red"] if "red" in colors else 0
        self.green = colors["green"] if "green" in colors else 0
        self.blue = colors["blue"] if "blue" in colors else 0
    
    def __repr__(self) -> str:
        return f"red: {self.red}, green: {self.green}, blue: {self.blue}"

    def is_valid(self) -> bool:
        return self.red <= self.MAX_RED and self.green <= self.MAX_GREEN and self.blue <= self.MAX_BLUE

def main():
    sum = 0
    with open("input.txt") as input:
        for id, line in enumerate(input):
            id += 1
            hands_part = line.split(":")[1].strip()
            hand_strings = [hand.strip() for hand in hands_part.split(";")]
            valid = True
            for string in hand_strings:
                cubes = [cube.strip() for cube in string.split(",")]
                counts_and_colors = {color: int(count) for count, color in [cube.split() for cube in cubes]}
                hand = Hand(counts_and_colors)
                if not hand.is_valid():
                    valid = False
                #print(id, hand, hand.is_valid())
            if valid:
                sum += id

                
    print(sum)



if __name__ == "__main__":
    main()