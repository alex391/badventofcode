use std::fs;

const CYCLES: usize = 220;
fn main() {
    let contents = fs::read_to_string("input.txt").expect("Should have been able to read the file");
    let contents = contents.lines();

    let mut signal_strength_sum: i32 = 0;

    let mut x: i32 = 1;
    let mut cycle_count = 0;
    let cool_signal_strengths: Vec<usize> = (20..=CYCLES).step_by(40).collect(); // the signal strengths that the puzzle wants to know about

    'program: for line in contents {
        // Increase the count
        let cycles = if line.starts_with("noop") { 1 } else { 2 };
        for _ in 0..cycles {
            cycle_count += 1;
            if cool_signal_strengths.contains(&cycle_count) {
                signal_strength_sum += cycle_count as i32 * x;
                if cycle_count == CYCLES {
                    break 'program;
                }
            }
        }
        // Actually do the addition, if applicable
        if !line.starts_with("noop") {
            let mut split = line.split(" ");
            x += split.nth(1).unwrap().parse::<i32>().unwrap();
        }
    }
    println!("{signal_strength_sum}");
}
