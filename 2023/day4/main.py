# OK, I acknowledge that this is a silly solution... keeping track of the count
# for each Scratchcard would have been enough
# Runs *a lot* faster in pypy3

class Scratchcard():
    score_cache = None

    def __init__(self, card_number: int, winning: set, have: list):
        self.card_number = card_number
        self.winning = winning
        self.have = have

    def __repr__(self) -> str:
        return f"card_number: {self.card_number}"

    def score(self) -> int:
        if self.score_cache is not None:
            return self.score_cache
        count = 0
        for n in self.have:
            if n in self.winning:
                count += 1
        self.score_cache = count
        return count


class LinkedList():
    _head = None
    _tail = None
    _length = 0

    class Node():
        def __init__(self, data):
            self.data = data
            self.next = None

        def __repr__(self):
            return repr(self.data)

    @property
    def head(self):
        return self._head

    @property
    def tail(self):
        return self._tail

    def __init__(self, initial_data: list = []):
        for d in initial_data:
            self.append(d)

    def __len__(self):
        return self._length

    def __repr__(self) -> str:
        string = ""
        current = self._head
        while True:
            string += repr(current.data)
            string += "\n"
            if current.next is not None:
                current = current.next
            else:
                break
        return string

    def insert_after(self, previous: Node, data) -> Node:
        """Take in the node to insert after, the data to insert, and return the
        new node"""
        if previous == self._tail:
            self.append(data)
            return self._tail

        new_node = self.Node(data)

        temp_node = previous.next
        previous.next = new_node
        new_node.next = temp_node

        self._length += 1
        return new_node

    def append(self, data):
        new_node = self.Node(data)
        if self._head is None:
            self._head = new_node
            self._tail = new_node
            self._length = 1
            return
        self._tail.next = new_node
        self._tail = new_node
        self._length += 1


def main():
    card_list = [None]  # None padding for to make index == card_number
    with open("input.txt") as file:
        for card_number, line in enumerate(file):
            card_number += 1
            card = line.split(": ")[1]
            winning, have = card.split(" | ")
            winning = {int(number) for number in winning.split()}
            have = [int(number) for number in have.split()]
            card_list.append(Scratchcard(card_number, winning, have))

    cards = LinkedList(card_list[1:])
    current_card = cards.head
    while True:
        score = current_card.data.score()
        temp = current_card
        if current_card.next is not None:
            for i in range(score):
                i += 1
                current_card = cards.insert_after(
                    current_card, card_list[temp.data.card_number + i])
        current_card = temp
        if current_card.next is not None:
            current_card = current_card.next
        else:
            break
    print(len(cards))


if __name__ == "__main__":
    main()
