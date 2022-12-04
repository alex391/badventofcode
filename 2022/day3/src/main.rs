use std::fs;
use substring::Substring;

fn main() {
    let contents = fs::read_to_string("input.txt")
        .expect("Should have been able to read the file");
    let contents = contents.lines();

    let mut rucksacks: Vec<[&str; 2]> = Vec::new();
    for line in contents {
        let midpoint = line.len() / 2;
        let rucksack: [&str; 2] = [line.substring(0, midpoint), line.substring(midpoint, line.len())];
        rucksacks.push(rucksack);
    }
    let mut priority_sum = 0;
    for rucksack in rucksacks {
        priority_sum += priority(find_misplaced(rucksack));
    }
    println!("{priority_sum}");

}
/// Return the letter that is similar between the sacks
fn find_misplaced(rucksack: [&str; 2]) -> char {
    for c in rucksack[0].chars() {
        if rucksack[1].contains(c){
            return c;
        }
    }
    panic!("Something went wrong finding the letter that was misplaced!")
}

/// Calculate the priority
fn priority(item: char) -> u32 {
    // this is a unicode hack
    let lower_offset: u32 = 'a' as u32;
    let upper_offset: u32 = 'A' as u32;
    if item.is_lowercase() {
        let item = item as u32;
        return (item - lower_offset) + 1;
    } else {
        let item = item as u32;
        return (item - upper_offset) + 27;
    }

}