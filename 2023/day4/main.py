def main():
    with open("input.txt") as file:
        sum = 0
        for line in file:
            card = line.split(": ")[1]
            winning, have = card.split(" | ")
            winning = {int(number) for number in winning.split()}
            have = [int(number) for number in have.split()]
            count = 0
            for n in have:
                if n in winning:
                    count += 1
            product = 1 if count > 0 else 0
            for _ in range(count - 1 if count > 0 else 0):
                product *= 2
            sum += product
            # print(winning, have, "count:", count, "score:", product)
        print(sum)
            



if __name__ == "__main__":
    main()