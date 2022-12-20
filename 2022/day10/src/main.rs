use std::fs;

fn main() {
    let contents = fs::read_to_string("input.txt").expect("Should have been able to read the file");
    let contents = contents.lines();

    let mut x: i32 = 1;
    let mut cycle_count = 0;

    const CRT_WIDTH: usize = 40;
    const CRT_HEIGHT: usize = 6;

    let mut crt: [bool; CRT_WIDTH * CRT_HEIGHT] = [false; CRT_WIDTH * CRT_HEIGHT];

    for line in contents {
        // Increase the count
        let cycles = if line.starts_with("noop") { 1 } else { 2 };
        for _ in 0..cycles {
            if (cycle_count % CRT_WIDTH) as i32 >= (x - 1)
                && (cycle_count % CRT_WIDTH) as i32 <= (x + 1)
            {
                crt[cycle_count] = true;
            }
            cycle_count += 1;
        }
        // Actually do the addition, if applicable
        if !line.starts_with("noop") {
            let mut split = line.split(' ');
            x += split.nth(1).unwrap().parse::<i32>().unwrap();
        }
    }

    println!("Part 2:");
    for pixel in crt.iter().enumerate() {
        print!("{}", if *pixel.1 { '#' } else { ' ' });
        if pixel.0 > 0 && (pixel.0 + 1) % CRT_WIDTH == 0 {
            println!();
        }
    }
}
