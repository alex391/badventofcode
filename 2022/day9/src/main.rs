use std::{collections::HashSet, fs};
const ROPE_LENGTH: usize = 10; // 2 for part 1, 10 for part 2

/// Determine if the head and the tail are touching  
/// Return true if they are, else false  
/// For example:
/// ```
/// let head_position = (5, 2);
/// let tail_position = (5, 0);
/// assert_eq!(touching(head_position, tail_position), false);
fn touching(head_position: (i32, i32), tail_position: (i32, i32)) -> bool {
    // if they are more than one away from each other in either direction
    (head_position.0 - tail_position.0).abs() <= 1 && (head_position.1 - tail_position.1).abs() <= 1
}
/// move the tail one closer to the head  
/// that is,  
/// ```
/// tail_position = movement(head_position, tail_position)
/// ```
fn movement(head_position: (i32, i32), tail_position: (i32, i32)) -> (i32, i32) {
    let mut tail_vector = (
        head_position.0 - tail_position.0,
        head_position.1 - tail_position.1,
    );
    // (0, 0) - (0, -2) = (0, 2), so go up 1
    // (0, 0) - (-2, -1) = (2, 1) so go up 1 and right 1
    // (0, 0) - (2, 0) = (-2, 0) so go down 1
    // Put the point through the sign function (can only move once in any direction)
    tail_vector = (
        if tail_vector.0 == 0 {
            0
        } else if tail_vector.0.is_positive() {
            1
        } else {
            -1
        },
        if tail_vector.1 == 0 {
            0
        } else if tail_vector.1.is_positive() {
            1
        } else {
            -1
        },
    );
    (
        tail_position.0 + tail_vector.0,
        tail_position.1 + tail_vector.1,
    )
}
fn main() {
    let contents = fs::read_to_string("input.txt").expect("Should have been able to read the file");
    let contents = contents.lines();

    let mut node_positions: [(i32, i32); ROPE_LENGTH] = [(0, 0); ROPE_LENGTH];

    let mut previous_tail_positions: HashSet<(i32, i32)> = HashSet::new();
    previous_tail_positions.insert(node_positions[ROPE_LENGTH - 1]); // include the starting location

    for line in contents {
        let mut split = line.split(' ');
        let mut head_vector: (i32, i32); // How far the head should move from it's current position

        // Update the head position:
        match split.next() {
            Some("U") => head_vector = (0, split.next().unwrap().parse::<i32>().unwrap()),
            Some("D") => head_vector = (0, -split.next().unwrap().parse::<i32>().unwrap()),
            Some("L") => head_vector = (-split.next().unwrap().parse::<i32>().unwrap(), 0),
            Some("R") => head_vector = (split.next().unwrap().parse::<i32>().unwrap(), 0),
            Some(&_) | None => panic!("Incorrect Direction!"),
        }

        // x movement
        while head_vector.0 != 0 {
            if head_vector.0 > 0 {
                node_positions[0].0 += 1;
                head_vector.0 -= 1;
            } else {
                node_positions[0].0 -= 1;
                head_vector.0 += 1;
            }
            for i in 1..ROPE_LENGTH {
                if !touching(node_positions[i - 1], node_positions[i]) {
                    node_positions[i] = movement(node_positions[i - 1], node_positions[i])
                }
            }
            previous_tail_positions.insert(node_positions[ROPE_LENGTH - 1]);

        }
        // y movement
        while head_vector.1 != 0 {
            if head_vector.1 > 0 {
                node_positions[0].1 += 1;
                head_vector.1 -= 1;
            } else {
                node_positions[0].1 -= 1;
                head_vector.1 += 1;
            }
            for i in 1..ROPE_LENGTH {
                if !touching(node_positions[i - 1], node_positions[i]) {
                    node_positions[i] = movement(node_positions[i - 1], node_positions[i])
                }
            }
            previous_tail_positions.insert(node_positions[ROPE_LENGTH - 1]);

        }
    }
    println!("Part 2: {}", previous_tail_positions.len())
}
