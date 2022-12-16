# I've decided to do day 7 in python first, and come back to do it in rust later
class File:
    def __init__(self, name, children, parent, size):
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


def main():
    contents = open("input.txt").read()
    contents = contents.splitlines()
    contents.pop(0)  # skip the 'cd /'

    root = File("/", [], None, 0)
    current_dir = root
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
            current_dir.children.append(File(split[1], [], current_dir, 0))
        elif not line.startswith("$ ls"):
            current_dir.children.append(File(split[1], [], current_dir, int(split[0])))
    print(len(root.children))
    print(find(root, "fjf").size)

if __name__ == "__main__":
    main()
