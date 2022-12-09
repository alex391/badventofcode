use std::{fs};

fn main() {

    let contents = fs::read_to_string("input.txt")
        .expect("Should have been able to read the file");

    println!("Part 1:");
    
    let contents = contents.lines();
    let mut max_calories: u32 = 0;
    let mut cur_calories: u32 = 0;
    for item in contents.clone() {
        if item.is_empty() {
            cur_calories = 0;
        } else {
            cur_calories += item.trim().parse::<u32>().unwrap();
            if cur_calories > max_calories {
                max_calories = cur_calories;
            }
        }
    }
    println!("{max_calories}");

    println!("Part 2:");
    // The first part was just finding a max. For the second one, I'll just sort.
    let mut elves: Vec<u32> = Vec::new();
    cur_calories = 0;
    for item in contents {
        if item.is_empty() {
            elves.push(cur_calories);
            cur_calories = 0;
        } else {
            cur_calories += item.trim().parse::<u32>().unwrap();
        }
    }
    elves.push(cur_calories); // no newline at the end of the file
    elves.sort_unstable_by(|a, b| b.cmp(a)); // needs to be reversed
    let sum = elves[0] + elves[1] + elves[2];
    println!("{sum}");

}
