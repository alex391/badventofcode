import sys
import operator
NUMBER_WORDS = ["zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"]
WORDS_AND_NUMBERS = NUMBER_WORDS + [str(n) for n in range(1, 10)]
def main():
    sum = 0
    with open("input.txt") as input:
        for line in input:
            try:
                first = get_calibration_number(line, True)
                last = get_calibration_number(line, False)
            except ValueError as v:
                print(v)
                return
            sum += (10 * first) + last
    print("Part 2:", sum)


def get_calibration_number(string: str, first: bool) -> int:
    op = operator.lt if first else operator.gt
    min_or_max_index = sys.maxsize if first else -1
    value = 0
    for i, item in enumerate(WORDS_AND_NUMBERS):
        try:
            index = string.index(item) if first else string.rindex(item)
            if op(index, min_or_max_index):
                min_or_max_index = index
                if item[0].isdecimal():
                    value = int(item)
                else:
                    value = i
        except ValueError:
            continue
    if min_or_max_index == -1 or min_or_max_index == sys.maxsize:
        raise ValueError(f'The string: "{string}" does not contain any numbers.')
    return value

 
                    

if __name__ == "__main__":
    main()