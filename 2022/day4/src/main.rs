use std::fs;

fn main() {
    let contents = fs::read_to_string("input.txt")
        .expect("Should have been able to read the file");
    let contents = contents.lines();


    let mut elf_paris: Vec<[u32; 4]> = Vec::new();
    for line in contents {
        let mut pair:[u32; 4] = [0; 4];
        let split_line:Vec<&str> = line.split(&[',','-']).collect();

        for i in 0..pair.len() {
            pair[i] = split_line[i].parse().unwrap();
        }

        elf_paris.push(pair);
    }

    let mut count = 0;
    for pair in elf_paris {
        if check_bounds(pair) {
            count += 1;
        }
    }
    println!("{count}");
}

/// check the pairs overlap
fn check_bounds(pair:[u32; 4]) -> bool{
    let upper_range = pair[2]..=pair[3];
    let lower_range = pair[0]..=pair[1];
    upper_range.contains(&pair[0]) && upper_range.contains(&pair[1]) ||
    lower_range.contains(&pair[2]) && lower_range.contains(&pair[3])
}
