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

    let mut current_keys_and_values: Vec<(String, (String, String))> = {
        let current_keys: Vec<String> = maps.keys().filter_map(|key| { 
            if key.ends_with('A') { Some(key.clone()) } else { None }
        }).collect(); // All the strings that end with A

        let mut current_values: Vec<(String, String)> = Vec::new();
        for key in &current_keys {
            current_values.push((maps.get(key).expect("{key} not in maps")).clone());
        }
        current_keys.iter().zip(current_values).map(|(k, v)| (k.clone(), v)).collect()
    };
    let mut count = 0;
    'outer: loop {
        for direction in &directions {
            let mut new_keys_and_values: Vec<(String, (String, String))> = Vec::new();
            for item in current_keys_and_values {
                new_keys_and_values.push(follow_map(direction, item.0, item.1, &maps));
                // break if all of the new keys end with Z
            }
            current_keys_and_values = new_keys_and_values;
            count += 1;
            if all_zs(&current_keys_and_values) {
                break 'outer;
            }
        }
    }
    println!("{count}");

}
/// Return true if all of the keys end with Zs
fn all_zs(kv: &Vec<(String, (String, String))>) -> bool {
    for (k, _) in kv {
        if !k.ends_with('Z') {
            return false;
        }
    }
    true
}

fn follow_map(
    direction: &char,
    current_key: String,
    current_value: (String, String),
    maps: &HashMap<String, (String, String)>,
) -> (String, (String, String)) {
    let new_key: String = match direction {
        'L' => {
            current_value.0.to_string()
        }
        'R' => {
            current_value.1.to_string()
        }
        _ => {
            panic!("Invalid direction");
        }
    };
    let new_value = maps
        .get(&new_key)
        .unwrap_or_else(|| panic!("{} not in maps", current_key));
    (new_key, new_value.clone())
}
