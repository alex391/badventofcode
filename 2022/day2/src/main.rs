use std::fs;

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
        let split: Vec<&str> = line.split(' ').collect();
        pairs.push(split);
    }
    let mut score = 0;
    for pair in pairs.clone() {
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
    println!("Part 1: {score}");

    score = 0;
    for pair in pairs {
        // determine if we win
        match pair[1] {
            "Z" => {
                score += 6
            }
            "Y" => {
                score += 3
            }
            "X" => (),
            _ => panic!("Incorrect code")
        }
        // then add the score for what we threw
        match play(pair) {
            "A" => {
                score += 1;
            },
            "B" => {
                score += 2;
            },
            "C" => {
                score += 3;
            },
            _ => panic!("Incorrect code") 
        }
    }
    println!("Part 2: {score}");

}

/// Determine if we won this game.
fn won(pair: Vec<&str>) -> OUTCOME {
    match pair[0] {
        "A" => match pair[1] { 
            "X" => OUTCOME::Tie,
            "Y" => OUTCOME::Win,
            "Z" => OUTCOME::Loss,
            _ => panic!("Incorrect code")
        },
        "B" => match pair[1] { 
            "X" => OUTCOME::Loss,
            "Y" => OUTCOME::Tie,
            "Z" => OUTCOME::Win,
            _ => panic!("Incorrect code")
        },
        "C" => match pair[1] { 
            "X" => OUTCOME::Win,
            "Y" => OUTCOME::Loss,
            "Z" => OUTCOME::Tie,
            _ => panic!("Incorrect code")
        },
        _ => panic!("Incorrect code")
    }
}

/// based on the pair, determine what to play next
fn play(pair: Vec<&str>) -> &str {
    // X means throw, Y means draw, Z means win
    match pair[0] {
        "A" => match pair[1] { // Rock
            "X" => "C",
            "Y" => "A",
            "Z" => "B",
            _ => panic!("Incorrect code")
        },
        "B" => match pair[1] {  // Paper
            "X" => "A",
            "Y" => "B",
            "Z" => "C",
            _ => panic!("Incorrect code")
        },
        "C" => match pair[1] { // Scissors
            "X" => "B",
            "Y" => "C",
            "Z" => "A",
            _ => panic!("Incorrect code")
        },
        _ => panic!("Incorrect code")
    }
}
