# I've decided to do day 7 in python first, and come back to do it in rust later
class File:
    def __init__(self, name: str, children: list, parent, size: int):
        self.name = name
        self.children = children
        self.parent = parent
        self.size = size


def find(file: File, name: str) -> File:
    """
    Find the file that is a child of this file, by name
    :param file: the directory to search through
    :param name: the name to search for
    :return: the file that was found
    """
    for child in file.children:
        if child.name == name:
            return child


def du(directory: File) -> int:
    """
    Traverse the tree below a directory, and get the Disk Usage of that directory
    :param directory: the directory to find traverse
    :return: the disk usage
    """
    size = 0
    queue = [directory]
    while len(queue) != 0:
        n = len(queue)
        while n > 0:
            file = queue.pop(0)
            size += file.size
            for child in file.children:
                queue.append(child)
            n -= 1
    return size


def main():
    contents = open("input.txt").read()
    contents = contents.splitlines()
    contents.pop(0)  # skip the 'cd /'

    root = File("/", [], None, 0)
    current_dir = root
    directories = [root]  # A list of all the directories
    # Build the tree:
    for line in contents:
        split = line.split(" ")
        if line.startswith("$ cd"):
            # Change to the directory with the right name
            if split[2] == "..":
                current_dir = current_dir.parent
            else:
                current_dir = find(current_dir, split[2])
        elif line.startswith("dir"):
            # add a new directory
            directory = File(split[1], [], current_dir, 0)
            current_dir.children.append(directory)
            directories.append(directory)
        elif not line.startswith("$ ls"):
            current_dir.children.append(File(split[1], [], current_dir, int(split[0])))
        # Just skip '$ ls'
    sizes = []
    for directory in directories:
        sizes.append(du(directory))
    bytes_sum = sum(filter(lambda x: x <= 100000, sizes))
    print(bytes_sum)


if __name__ == "__main__":
    main()
