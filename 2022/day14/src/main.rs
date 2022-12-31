use std::{cmp, fs};

fn main() {
    let contents = fs::read_to_string("input.txt").expect("Should have been able to read the file");
    let contents = contents.lines();

    // Determine what the height should be
    let mut grid_height: usize = 0;
    for line in contents.clone() {
        let height: usize = line
            .split([' ', ',', '-', '>'])
            .filter_map(|x| x.parse::<usize>().ok()).skip(1).step_by(2)
            .max()
            .unwrap();
        grid_height = cmp::max(height, grid_height);
    }
    grid_height += 3; // Space for the floor
    let grid_width: usize = (grid_height * 2) + 500; // This is just worst case of how big it can be
    let mut grid: Vec<Vec<bool>> = vec![vec![false; grid_width]; grid_height];

    // Place the rocks
    for line in contents {
        let coordinates: Vec<usize> = line
            .split([' ', ',', '-', '>'])
            .filter_map(|x| x.parse::<usize>().ok())
            .collect();
        let mut last: (usize, usize) = (coordinates[0], coordinates[1]);
        let mut i = 2;
        while i < coordinates.len() {
            let current: (usize, usize) = (coordinates[i], coordinates[i + 1]); // need to start at the lower number here
            for x in cmp::min(last.0, current.0)..=cmp::max(last.0, current.0) {
                #[allow(clippy::needless_range_loop)] // this is more readable.
                for y in cmp::min(last.1, current.1)..=cmp::max(last.1, current.1) {
                    grid[y][x] = true;
                }
            }
            last = current;
            i += 2;
        }
    }
    // Place the bottom rock
    for x in 0..grid_width {
        grid[grid_height - 1][x] = true;
    }

    let mut count = 0;

    'end: loop {
        // Loop until a piece of sand reaches the end
        let mut sand_position: (usize, usize) = (500, 0); // (x, y) (but keep in mind that the grid is indexed by [y][x])
        loop {
            // loop until the piece of sand comes to rest
            if !grid[sand_position.1 + 1][sand_position.0] {
                // Move down if it can. grid[y][x] == false if you can go there
                sand_position.1 += 1;
            } else if !grid[sand_position.1 + 1][sand_position.0 - 1] {
                // Then try to move left and down
                sand_position.0 -= 1;
                sand_position.1 += 1;
            } else if !grid[sand_position.1 + 1][sand_position.0 + 1] {
                sand_position.0 += 1;
                sand_position.1 += 1;
            } else {
                // The sand has come to rest
                grid[sand_position.1][sand_position.0] = true;
                count += 1;

                // Printing the sand every step of the way. Commented out because the full input takes too long to print
                /* for y in &grid {
                    for x in y {
                        print!("{}", if *x { '#' } else {'.'});
                    }
                    println!();
                }
                println!();
                */

                if sand_position == (500, 0) {
                    break 'end;
                } else {
                    break;
                }
            }
        }
    }
    println!("Part 2: {count}");
}
