from enum import Enum
from collections import Counter


class HandType(Enum):
    FIVE_OF_A_KIND = 7
    FOUR_OF_A_KIND = 6
    FULL_HOUSE = 5
    THREE_OF_A_KIND = 4
    TWO_PAIR = 3
    ONE_PAIR = 2
    HIGH_CARD = 1

    def __lt__(self, other) -> bool:
        return self.value < other.value


CARD_VALUES = dict([(item[1], item[0]) for item in enumerate(['2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'])]) # Dictionary from card to it's value
class Hand():
    
    _hand_type = None

    @property
    def hand_type(self) -> HandType:
        if self._hand_type != None:
            return self._hand_type
        card_counts = list(Counter(self.hand).values())
        card_counts.sort()
        card_counts = card_counts[::-1]
        card_counts_max = card_counts[0]
        match card_counts_max:
            case 5:
                self._hand_type = HandType.FIVE_OF_A_KIND
            case 4:
                self._hand_type = HandType.FOUR_OF_A_KIND
            case 3:
                if card_counts[1] == 2:
                    self._hand_type = HandType.FULL_HOUSE
                else:
                    self._hand_type = HandType.THREE_OF_A_KIND
            case 2:
                if card_counts[1] == 2:
                    self._hand_type = HandType.TWO_PAIR
                else:
                    self._hand_type = HandType.ONE_PAIR
            case 1:
                self._hand_type = HandType.HIGH_CARD
            case _:
                print(f"Shouldn't be possible for card_counts_max to be {card_counts_max}")
                exit()
        return self._hand_type
    

    def __init__(self, hand: str, bid: str):
        self.hand = str.strip(hand)
        self.bid = int(bid)

    def __repr__(self) -> str:
        return f"{self.hand} {self.bid} {self.hand_type}"
    
    def _hand_strength_lt(self, other) -> bool:
        for item in zip(self.hand, other.hand):
            if CARD_VALUES[item[0]] < CARD_VALUES[item[1]]:
                return True
            elif CARD_VALUES[item[0]] > CARD_VALUES[item[1]]:
                return False
        print("Something's gone wrong with _hand_strength_lt") # Should never be equal? Might be better to return False
        exit()
    
    def __lt__(self, other) -> bool:
        if self.hand_type != other.hand_type:
            return self.hand_type < other.hand_type
        else:
            return self._hand_strength_lt(other)
            

def main():
    hands = []
    with open("input.txt") as file:
        for line in file:
            hands.append(Hand(*line.split(" ")))

    hands.sort()

    for hand in hands:
        print(hand)

    sum = 0
    rank = 1
    i = 0
    while i < len(hands): # Not very pythonic, but my brain hurts trying to think about enumerate here
        sum += hands[i].bid * rank
        i += 1
        rank += 1
    print(sum)

if __name__ == "__main__":
    main()