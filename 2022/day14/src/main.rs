use std::{fs, cmp};

fn main() {
    let contents = fs::read_to_string("input.txt").expect("Should have been able to read the file");
    let contents = contents.lines();
    const GRID_HEIGHT: usize = 200;
    const GRID_WIDTH: usize = 700;
    let mut grid: [[bool; GRID_WIDTH]; GRID_HEIGHT] = [[false; GRID_WIDTH]; GRID_HEIGHT]; // Literally just guessing about how big the grid needs to be.

    for line in contents {
        let coordinates: Vec<usize> = line.split([' ', ',', '-', '>']).filter_map(|x| x.parse::<usize>().ok()).collect();
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


    let mut count = 0;

    'end: loop { // Loop until a piece of sand reaches the end
        let mut sand_positon: (usize, usize) = (500, 0); // (x, y) (but keep in mind that the grid is indexed by [y][x])
        loop { // loop until the piece of sand comes to rest
            if !grid[sand_positon.1 + 1][sand_positon.0] { // Move down if it can. grid[y][x] == false if you can go there
                sand_positon.1 += 1;
            } else if !grid[sand_positon.1 + 1][sand_positon.0 - 1]{ // Then try to move left and down
                sand_positon.0 -= 1;
                sand_positon.1 += 1;
            } else if !grid[sand_positon.1 + 1][sand_positon.0 + 1] {
                sand_positon.0 += 1;
                sand_positon.1 += 1;
            }
            
            else { // The sand has come to rest
                grid[sand_positon.1][sand_positon.0] = true;
                count += 1;
                
                break;
            }

            if sand_positon.1 == GRID_HEIGHT - 1 {
                break 'end;
            }
        }

    }
    println!("Part 1: {count}");

}

