use regex::{self, Regex};
use std::{
    fs, collections::HashMap,
};

fn main() {
    let contents = fs::read_to_string("input.txt").expect("Should have been able to read the file");
    let mut contents = contents.lines();

    let directions: Vec<char> = contents.next().expect("Directions should exist!").trim().chars().collect();
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
        maps.insert(node_split[0].to_string(), (node_split[1].to_string(), node_split[2].to_string()));
    }
    

    let mut current_key: String = "AAA".to_string();
    let mut current_value: &(String, String) = maps.get(&current_key).unwrap_or_else(|| panic!("{current_key} not in maps"));
    let mut count = 0;
    'outer: loop {
        for c in &directions {
            match c {
                'L' => {
                    current_key = current_value.0.to_string();
                }
                'R' => {
                    current_key = current_value.1.to_string();
                }
                _ => {
                    panic!("Invalid direction");
                }
            }
            count += 1;
            if current_key == "ZZZ" {
                break 'outer;
            }
            current_value = maps.get(&current_key).unwrap_or_else(|| panic!("{current_key} not in maps"));
        }
    }

    println!("{count}");
}

