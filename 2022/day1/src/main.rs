use std::{fs};

fn main() {

    let contents = fs::read_to_string("input.txt")
        .expect("Should have been able to read the file");

    println!("Part 1:");
    
    let contents = contents.lines();
    let mut max_calories: u32 = 0;
    let mut cur_calories: u32 = 0;
    for item in contents {
        if item == ""{
            cur_calories = 0;
        }
        else {
            cur_calories = cur_calories + item.trim().parse::<u32>().unwrap();
            if cur_calories > max_calories {
                max_calories = cur_calories;
            }
        }

    }
    println!("{max_calories}");

    println!("Part 2")
    // The first part was just finding a max. For the second one, I'll just sort.


}
