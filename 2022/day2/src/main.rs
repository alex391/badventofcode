use std::{fs};

enum OUTCOME  {
    Win,
    Loss,
    Tie
}
fn main() {
    let contents = fs::read_to_string("input.txt")
        .expect("Should have been able to read the file");

    // A, X is for Rock
    // B, Y is for Paper
    // C, Z is for Scissors
    // rock > scissors
    // If you choose:
    // Rock: 1pt
    // Paper: 2pts
    // Scissors: 3pts
    // Win: 6pts, Draw: 3pts, Loss: 0pts

    let contents = contents.lines();
    let mut pairs: Vec<Vec<&str>> = Vec::new();
    for line in contents {
        let split: Vec<&str> = line.split(" ").collect();
        pairs.push(split);
    }
    let mut score = 0;
    for pair in pairs {
        match pair[1] {
            "X" => {
                score += 1;
            },
            "Y" => {
                score += 2;
            },
            "Z" => {
                score += 3;
            },
            _ => panic!("Incorrect code") 
        }
        match won(pair) {
            OUTCOME::Win => {
                score += 6;
            },
            OUTCOME::Tie => {
                score += 3;
            },
            OUTCOME::Loss => () // no need to add zero
        }
    }
    println!("{score}")
}

/// Determine if we won this game.
fn won(pair: Vec<&str>) -> OUTCOME {
    match pair[0] {
        "A" => match pair[1] { 
            "X" => return OUTCOME::Tie,
            "Y" => return OUTCOME::Win,
            "Z" => return OUTCOME::Loss,
            _ => panic!("Incorrect code")
        },
        "B" => match pair[1] { 
            "X" => return OUTCOME::Loss,
            "Y" => return OUTCOME::Tie,
            "Z" => return OUTCOME::Win,
            _ => panic!("Incorrect code")
        },
        "C" => match pair[1] { 
            "X" => return OUTCOME::Win,
            "Y" => return OUTCOME::Loss,
            "Z" => return OUTCOME::Tie,
            _ => panic!("Incorrect code")
        },
        _ => panic!("Incorrect code")
    };
}
