use regex::Regex;
use std::fs;

fn part_one(contents: &str) {
    let re = Regex::new(r"mul\(\d*,\d*\)").expect("Problem with regex!");
    let matches = re.find_iter(contents);
    let mut sum = 0;
    for m in matches {
        // Unwrap because we already know it starts with mul( and ends with )
        let mut split = m
            .as_str()
            .strip_prefix("mul(")
            .unwrap()
            .strip_suffix(")")
            .unwrap()
            .split(',');
        let lhs: i32 = split.next().unwrap().parse().unwrap();
        let rhs: i32 = split.next().unwrap().parse().unwrap();
        sum += lhs * rhs;
    }
    println!("{sum}")
}

fn part_two(contents: &str) {
    let re = Regex::new(r"mul\(\d*,\d*\)|do\(\)|don't\(\)").expect("Problem with regex!");
    let matches = re.find_iter(contents);
    let mut sum = 0;
    let mut enabled = true;
    for m in matches {
        match m.as_str().chars().collect::<Vec<char>>()[2] {
            'l' => {
                if !enabled {
                    continue;
                }
                let mut split = m
                    .as_str()
                    .strip_prefix("mul(")
                    .unwrap()
                    .strip_suffix(")")
                    .unwrap()
                    .split(',');
                let lhs: i32 = split.next().unwrap().parse().unwrap();
                let rhs: i32 = split.next().unwrap().parse().unwrap();
                sum += lhs * rhs;
            }
            '(' => {
                enabled = true;
            }
            'n' => {
                enabled = false;
            }
            _ => unreachable!(),
        }
    }
    println!("{sum}")
}

fn main() {
    let contents = fs::read_to_string("input.txt").expect("Can't read input!");
    part_one(&contents);
    part_two(&contents);
}
