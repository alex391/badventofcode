# Another dumb solution: brute force! (read: repo name)

from multiprocessing import Pool as ThreadPool
class AlmanacMap():
    def __init__(self, to_parse: str):
        self.destination, self.source, self.length = [int(x) for x in to_parse.split()]
    
    def __repr__(self) -> str:
        return f"({self.destination}, {self.source}, {self.length})"

    def map_seed(self, input) -> (int, bool):
        if input in range(self.source, self.source + self.length):
            return input + self.destination - self.source, True
        return input, False

almanac = [] # list of list of AlmanacMaps
def main(): 
    with open("input.txt") as file:
        seeds = map(int, next(file).split()[1:])
        # Take seeds (iterable containing [1, 2, 3, 4]) and convert to ranges ([range(1, 3), range(3, 7)])
        seed_ranges = list(map(lambda x: range(x[0], x[0] + x[1]), zip(*(seeds,) * 2))) # Thanks https://stackoverflow.com/a/1625023 by https://stackoverflow.com/users/97828/nadia-alramli
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

    # CPU go brrr
    pool = ThreadPool(len(seed_ranges)) # Thanks https://stackoverflow.com/a/28463266 by https://stackoverflow.com/users/2327328/philshem
    lowest_location_numbers = pool.map(lowest_location_in_seed_range, seed_ranges)

    print(min(lowest_location_numbers))

def lowest_location_in_seed_range(seed_range: range) -> int:
    lowest_location_number = float("inf")
    for seed in seed_range:
            for almanac_maps in almanac:
                for almanac_map in almanac_maps:
                    seed, matched = almanac_map.map_seed(seed)
                    if matched:
                        break
            lowest_location_number = seed if seed < lowest_location_number else lowest_location_number
    return lowest_location_number



if __name__ == "__main__":
    main()

