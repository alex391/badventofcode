use std::{collections::VecDeque, fs};
struct Monkey {
    items: VecDeque<u64>,
    operation: char, // A char representing the operation to do, either *, +, or ^ for squaring
    operation_value: u64, // What to multiply or add
    divisible: u64,  // The value to check if it's divisible by
    throw_if_true: usize, // Where to throw it to
    throw_if_false: usize,
}

impl Monkey {
    fn new(
        items: Vec<u64>,
        operation: char,
        operation_value: u64,
        divisible: u64,
        throw_if_true: usize,
        throw_if_false: usize,
    ) -> Self {
        Self {
            items: VecDeque::from(items),
            operation,
            operation_value,
            divisible,
            throw_if_true,
            throw_if_false,
        }
    }

    fn push_back(&mut self, item: u64) {
        self.items.push_back(item)
    }

    fn pop_front(&mut self) -> Option<u64> {
        self.items.pop_front()
    }
}

const MONKEY_COUNT: usize = 8; // Not going to read this from the input
const ROUNDS: u32 = 10000; // The number of rounds to do

fn main() {
    let contents = fs::read_to_string("input.txt").expect("Should have been able to read the file");
    let mut contents = contents.lines();

    // First, parse the input
    let mut monkeys: Vec<Monkey> = Vec::new();

    for _ in 0..MONKEY_COUNT {
        // This input is *terrible*
        // Maybe I should have just put it in manually...
        let items: Vec<u64> = contents
            .nth(1)
            .unwrap()
            .split([' ', ','])
            .filter_map(|x| x.parse::<u64>().ok())
            .collect();
        let split_operation = contents.next().unwrap().split(' ');
        let operation: char = if split_operation.clone().filter(|x| x.eq(&"old")).count() > 1 {
            '^'
        } else {
            split_operation
                .clone()
                .find(|x| x.eq(&"*") || x.eq(&"+"))
                .unwrap()
                .chars()
                .next()
                .unwrap()
        };
        let operation_value: u64 = split_operation.last().unwrap().parse().unwrap_or_default(); // The old at the end of the ^ case will trip this up. That's ok, just use the default
        let divisible: u64 = contents
            .next()
            .unwrap()
            .split(' ')
            .last()
            .unwrap()
            .parse()
            .unwrap();
        let throw_if_true: usize = contents
            .next()
            .unwrap()
            .split(' ')
            .last()
            .unwrap()
            .parse()
            .unwrap();
        let throw_if_false: usize = contents
            .next()
            .unwrap()
            .split(' ')
            .last()
            .unwrap()
            .parse()
            .unwrap();
        contents.next(); // just consume the blank line

        monkeys.push(Monkey::new(
            items,
            operation,
            operation_value,
            divisible,
            throw_if_true,
            throw_if_false,
        ));
    }

    let mut divisible_product: u64 = 1;
    for monkey in &monkeys {
        divisible_product *= monkey.divisible;
    }

    // Solve the problem:
    let mut inspections: [u64; MONKEY_COUNT] = [0; MONKEY_COUNT];
    for _ in 0..ROUNDS {
        for i in 0..monkeys.len() {
            while !monkeys[i].items.is_empty() {
                let mut item = monkeys[i].pop_front().unwrap();
                // Inspect the item:
                inspections[i] += 1;
                match monkeys[i].operation {
                    '+' => {
                        item += monkeys[i].operation_value;
                    }
                    '*' => {
                        item *= monkeys[i].operation_value;
                    }
                    '^' => {
                        item = item.pow(2);
                    }
                    _ => panic!("Incorrect operation!"),
                }
                // Make the number more reasonable:
                item %= divisible_product;
                // Check divisibility
                if item % monkeys[i].divisible == 0 {
                    let throw_if_true: usize = monkeys[i].throw_if_true;
                    monkeys[throw_if_true].push_back(item);
                } else {
                    let throw_if_false: usize = monkeys[i].throw_if_false;
                    monkeys[throw_if_false].push_back(item);
                }
            }
        }
    }
    inspections.sort_unstable_by(|a, b| b.cmp(a));
    println!("Part 2: {}", inspections[0] * inspections[1]);
}
