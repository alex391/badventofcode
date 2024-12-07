from itertools import pairwise
from copy import deepcopy


class Report:
    def __init__(self, report: list[int]):
        self._report = report

    def is_safe(self) -> bool:
        # Is it the same when sorted?
        if self._report != sorted(self._report) and self._report != sorted(self._report, reverse=True):
            return False

        # https://stackoverflow.com/a/65190808
        differences = [abs(x - y) for (x, y) in pairwise(self._report)]
        if max(differences) > 3 or min(differences) < 1:
            return False
        return True

    def is_safe_problem_dampened(self) -> bool:
        # Early exit: it's already safe
        if self.is_safe():
            return True
        # But then check all of the variations with one removed
        variants = [deepcopy(self) for _ in self._report]
        for i, variant in enumerate(variants):
            del variant._report[i]
            if variant.is_safe():
                return True
        return False


def part_one():
    with open("input.txt", "r") as f:
        reports = []
        for line in f:
            reports.append(Report([int(item) for item in line.split()]))
    count = 0
    for report in reports:
        if report.is_safe():
            count += 1

    print(count)

def part_two():
    with open("input.txt", "r") as f:
        reports = []
        for line in f:
            reports.append(Report([int(item) for item in line.split()]))
    count = 0
    for report in reports:
        if report.is_safe_problem_dampened():
            count += 1

    print(count)


if __name__ == "__main__":
    part_one()
    part_two()
