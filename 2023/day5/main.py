class AlmanacMap():
    def __init__(self, to_parse: str):
        self.destination, self.source, self.length = [int(x) for x in to_parse.split()]
    
    def __repr__(self) -> str:
        return f"({self.destination}, {self.source}, {self.length})"

    def map_seed(self, input) -> (int, bool):
        if input in range(self.source, self.source + self.length):
            return input + self.destination - self.source, True
        return input, False
    



def main(): 
    with open("input.txt") as file:
        seeds = list(map(int, next(file).split()[1:]))
        almanac = [] # list of list of AlmanacMaps
        current_map = []
        for line in file:
            line = line.strip()
            if line == "":
                continue # just skip it
            if line[-1].isdigit():
                current_map.append(AlmanacMap(line))
            elif current_map:
                almanac.append(current_map)
                current_map = []
        almanac.append(current_map)

    for almanac_maps in almanac:
        matched = [False for _ in seeds]
        for almanac_map in almanac_maps:
            print(seeds)
            print(almanac_map)
            for i, seed in enumerate(seeds):
                if (matched[i]):
                   continue
                seeds[i], matched[i] = almanac_map.map_seed(seed)

    print(min(seeds))

if __name__ == "__main__":
    main()

