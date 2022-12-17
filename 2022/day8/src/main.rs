use std::fs;

/// Check if a tree is visible in that "row"
/// Return true if the tree is visible, otherwise false
fn check_visibility(row: &[u32], index: usize) -> bool {
    index == 0 || index == row.len() ||
    row.iter().enumerate().filter(|&x| x.0 < index && *x.1 >= row[index]).count() == 0 || //left side visibility
    row.iter().enumerate().filter(|&x| x.0 > index && *x.1 >= row[index]).count() == 0 // right side visibility
}

/// Calculate how many trees are visible, given a row to the right of it (or rotated so that it is)
fn visible_trees(row: &[u32], height: u32) -> usize {
    for tree in row.iter().enumerate() {
        if height <= *tree.1 {
            return tree.0 + 1;
        }
    }
    row.len()
}

fn main() {
    let contents = fs::read_to_string("input.txt").expect("Should have been able to read the file");
    let contents = contents.lines();
    let mut trees: Vec<Vec<u32>> = Vec::new();
    // Convert all of the chars to u32s
    for line in contents {
        let mut row: Vec<u32> = Vec::new();
        for c in line.chars() {
            row.push(c.to_digit(10).unwrap());
        }
        trees.push(row);
    }
    let mut count = 0;
    // 0, 0 is the top left corner of this array
    // 0, 1 is one below that
    // 1, 0 is one to the right
    for y in 0..trees.len() {
        for x in 0..trees[y].len() {
            // First, check visibility from the left side
            if check_visibility(&trees[y], x) {
                count += 1;
            } else {
                let mut column = Vec::new();
                for row in &trees {
                    column.push(row[x])
                }
                if check_visibility(&column[..], y) {
                    count += 1;
                }
            }
        }
    }
    println!("Part 1: {count}");

    let mut score = 0;
    for y in 0..trees.len() {
        for x in 0..trees[y].len() {
            let mut left = trees[y].clone(); // Don't want to mutate the original vec
            left = left[0..x].to_vec();
            left.reverse();
            let mut this_score = visible_trees(&left, trees[y][x]);// Left
            this_score *= visible_trees(&trees[y][(x + 1)..], trees[y][x]); //Right
            let mut column = Vec::new();
            for row in &trees {
                column.push(row[x])
            }
            let mut up = column.clone();
            up = up[0..y].to_vec();
            up.reverse();
            this_score *= visible_trees(&up, trees[y][x]); // Up
            this_score *= visible_trees(&column[(y + 1)..], trees[y][x]); // Down
            if this_score > score {
                score = this_score;
            }

        }
    }
    println!("Part 2: {score}");


}
