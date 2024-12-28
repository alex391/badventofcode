use std::fs;

use counter::Counter;

fn digits(n: u64) -> u32 {
    // https://stackoverflow.com/q/52276687
    n.ilog10() + 1
}

fn split_number(n: u64) -> (u64, u64) {
    let half_digits = digits(n) / 2;
    let left: u64 = n / 10_u64.pow(half_digits);
    let right: u64 = n - (10_u64.pow(half_digits) * left);
    (left, right)
}

fn main() {
    let contents = fs::read_to_string("input.txt").expect("Can't read input.txt!");
    let contents = contents.lines().next().expect("File is empty!");

    let stone_list = contents.split(' ').map(|x| x.parse::<u64>().unwrap());
    let mut stones: Counter<u64> = stone_list.collect();
    for _ in 0..75 {
        let mut new_stones: Counter<u64> = Counter::new();
        for (stone, count) in &stones {
            if *stone == 0 {
                new_stones[&1] += count;
            } else if digits(*stone) % 2 == 0 {
                let (left, right) = split_number(*stone);
                new_stones[&left] += count;
                new_stones[&right] += count;
            } else {
                new_stones[&(*stone * 2024)] += count;
            }
        }
        stones = new_stones;
    }
    let mut sum = 0;
    for count in stones.values() {
        sum += count;
    }
    println!("{sum}");
}
