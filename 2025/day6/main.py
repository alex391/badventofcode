import time

def read_file() -> list:
    table = list()

    with open("input.txt","r") as file:
        first = True
        for line in file:
            data = line.strip().split()
            i = 0
            for element in data:
                if first:
                    table.append(list())
                if element not in ["+","*"]:
                    table[i].append(int(element))
                else:
                    table[i].append(element)
                i += 1

            first = False

    return table

def part1_func(table:list) -> int:
    total = 0
    
    for column in table:
        if column[-1] == "*":
            product = 1
            for element in column:
                if element != "*":
                    product *= element
            total += product
        else:
            for element in column:
                if element != "+":
                    total += element
    
    return total

def part2_func() -> int:
    total = 0
    table = list()
    first = True

    #build the table
    with open("input.txt","r") as file:
        for line in file:
            i = 0

            #not final line
            if line[0] not in ["+","*"]:
                for char in line:
                    if first:
                        table.append(list())
                    table[i].append(char)
                    i += 1
            else: #final line
                i = 0
                add = True #arbitrary initial value
                product = 0
                for char in line:
                    if char == "+":
                        if not add:
                            total += product
                        add = True
                    elif char == "*":
                        if not add:
                            total += product
                        add = False
                        product = 1
                    
                    if add and char != "\n":
                        ceph_num = ""
                        for digit in table[i]:
                            if digit != " ":
                                ceph_num += digit
                        if ceph_num != "":
                            total += int(ceph_num)
                    elif char != "\n":
                        ceph_num = ""
                        for digit in table[i]:
                            if digit != " ":
                                ceph_num += digit
                        if ceph_num != "":
                            product *= int(ceph_num)


                    i += 1

                if not add:
                    total += product

            first = False

    return total

def main():
    start_time = time.perf_counter()

    table = read_file()
    part1 = part1_func(table)
    part2 = part2_func()

    end_time = time.perf_counter()

    print(f"Part 1: {part1}")
    print(f"Part 2: {part2}")
    print(f"Solution time: {end_time-start_time}")

main()