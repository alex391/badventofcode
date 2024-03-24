use regex::{self, Regex};
use std::{collections::HashMap, fs};

fn main() {
    let contents = fs::read_to_string("input.txt").expect("Should have been able to read the file");
    let mut contents = contents.lines();

    let directions: Vec<char> = contents
        .next()
        .expect("Directions should exist!")
        .trim()
        .chars()
        .collect();
    contents.next();

    let re = Regex::new(r" = \(|, |\)").expect("Invalid regex!");
    let mut maps: HashMap<String, (String, String)> = HashMap::new();
    for line in contents {
        let node_split: Vec<&str> = re
            .split(line)
            .filter_map(|x| {
                let x = x.trim();
                if x.is_empty() {
                    None
                } else {
                    Some(x)
                }
            })
            .collect();
        maps.insert(
            node_split[0].to_string(),
            (node_split[1].to_string(), node_split[2].to_string()),
        );
    }

    let starting_keys = maps.keys().filter(|x| x.ends_with('Z'));
    let mut counts: Vec<u64> = Vec::new();
    for key in starting_keys {
        let mut key: String = key.clone();
        let mut count = 0;
        let mut current_value: &(String, String) = maps
            .get(&key)
            .unwrap_or_else(|| panic!("{key} not in maps"));
        'outer: loop {
            for c in &directions {
                match c {
                    'L' => {
                        key = current_value.0.to_string();
                    }
                    'R' => {
                        key = current_value.1.to_string();
                    }
                    _ => {
                        panic!("Invalid direction");
                    }
                }
                count += 1;
                if key.ends_with('Z') {
                    break 'outer;
                }
                current_value = maps
                    .get(&key)
                    .unwrap_or_else(|| panic!("{key} not in maps"));
            }
        }
        counts.push(count);
    }
    println!("{}", vec_lcm(counts))
}

fn vec_lcm(mut x: Vec<u64>) -> u64 {
    match x.len() {
        0 => panic!("Can't lcm with empty Vec"),
        1 => x[0],
        2 => num::integer::lcm(x[0], x[1]),
        3.. => num::integer::lcm(x.pop().unwrap(), vec_lcm(x)),
    }
}
