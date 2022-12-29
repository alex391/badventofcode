use std::{cmp::Ordering, fs};

use json::JsonValue;
#[macro_use]
extern crate json;
fn main() {
    let contents = fs::read_to_string("input.txt").expect("Should have been able to read the file");
    // I'm going to parse this input as if it's a bunch of new line separated json strings
    let mut contents: Vec<JsonValue> = contents
        .lines()
        .filter_map(|line| {
            if !line.is_empty() {
                Some(json::parse(line).unwrap())
            } else {
                None
            }
        })
        .collect();

    let mut pair_index = 0;
    let mut aoc_one_indexed = 1;
    let mut in_order: Vec<usize> = Vec::new(); // All the indexes that are in the right order
    while pair_index < contents.len() {
        let left = &contents[pair_index];
        let right = &contents[pair_index + 1];
        if compare(left, right).is_lt() {
            in_order.push(aoc_one_indexed)
        }
        pair_index += 2;
        aoc_one_indexed += 1;
    }
    println!("Part 1: {}", in_order.iter().sum::<usize>());

    let divider_one = array![array![2]];
    let divider_two = array![array![6]];

    contents.push(divider_one.clone());
    contents.push(divider_two.clone());
    

    contents.sort_unstable_by(compare);

    let divider_one_index: usize = contents.iter().enumerate().find(|&x| *x.1 == divider_one).unwrap().0 + 1; 
    let divider_two_index: usize = contents.iter().enumerate().find(|&x| *x.1 == divider_two).unwrap().0 + 1;

    println!("Part 2: {}", divider_one_index * divider_two_index);


}

/// Less if left < right, Greater if right > left, else Equal  
/// I'd like to thank https://github.com/valogonor/advent-of-code/blob/main/2022/day13.py for pointing me in the right direction here
fn compare(left: &JsonValue, right: &JsonValue) -> Ordering {
    let mut result: Ordering = Ordering::Equal;
    if left.is_number() && right.is_number() {
        return left.as_u32().unwrap().cmp(&right.as_u32().unwrap());
    } else if left.is_array() && right.is_array() {
        let left_length = left.len();
        let right_length = right.len();
        for i in 0..std::cmp::min(left_length, right_length) {
            result = compare(&left[i], &right[i]);
            if result.is_ne() {
                break;
            }
        }
        if result.is_eq() {
            return left_length.cmp(&right_length);
        }
    } else if left.is_number() {
        result = compare(&array![left.clone()], right)
    } else {
        result = compare(left, &array![right.clone()])
    }
    result
}
