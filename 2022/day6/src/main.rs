use std::{collections::VecDeque, fs};

const MARKER_WIDTH: usize = 14;
fn main() {
    let contents = fs::read_to_string("input.txt").expect("Should have been able to read the file");

    let mut buffer: VecDeque<char> = VecDeque::with_capacity(MARKER_WIDTH);

    for letter in contents.chars().enumerate() {
        buffer.push_back(letter.1);
        if buffer.len() == MARKER_WIDTH {
            if test_uniqueness(&buffer) {
                println!("{}", letter.0 + 1); // off-by-one - needs ordinal not index
                for c in buffer {
                    print!("{c}");
                }
                break;
            }
            buffer.pop_front();
        }
    }
}

fn test_uniqueness(buffer: &VecDeque<char>) -> bool {
    let mut unique_count = 0;
    for letter in buffer {
        if buffer.iter().filter(|&c| *c == *letter).count() == 1 {
            unique_count += 1;
        }
    }
    unique_count == MARKER_WIDTH
}
