// Use cargo run --release for this!

use std::{fs, str::FromStr};
struct Calibration {
    expected: i64,
    equation: Vec<i64>,
}

impl FromStr for Calibration {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut split = s.split(&[' ', ':']).filter(|x| !x.is_empty());
        let expected: i64 = match split.next().ok_or("Can't parse test value")?.parse() {
            Ok(i) => i,
            Err(e) => return Err(e.to_string()),
        };

        let mut equation: Vec<i64> = Vec::new();
        for item in split {
            if let Ok(i) = item.parse() {
                equation.push(i);
            } else {
                return Err("Can't parse an integer!".to_string());
            }
        }

        Ok(Calibration { expected, equation })
    }
}

/// From the right (so get_trit(4, 0) returns 1)  
/// This is starting to remind me of a previous [SNAFU](https://adventofcode.com/2022/day/25)
fn get_trit(mut integer: u32, index: usize) -> u8 {
    // https://stackoverflow.com/a/3614138
    // Also this could potentially be cached, if performance is bad
    let mut trits: Vec<u8> = Vec::new();
    while integer >= 3 {
        let trit = integer % 3;
        trits.push(trit as u8);
        integer /= 3;
    }
    trits.push(integer as u8);
    trits.reverse();
    *trits.get(index).unwrap_or(&0)
}

fn concatenate(lhs: i64, rhs: i64) -> i64 {
    // Could convert to string and back but this is slightly nicer
    // https://stackoverflow.com/a/4483960
    let rhs_length = rhs.ilog10() + 1;
    (lhs * 10_i64.pow(rhs_length)) + rhs
}

fn main() {
    let contents = fs::read_to_string("input.txt").expect("Can't read input.txt!");
    let calibrations: Vec<Calibration> = contents.lines().map(|l| l.parse().unwrap()).collect();

    let mut sum = 0;
    for calibration in calibrations {
        // The idea is that we'll go through all of the combinations as if
        // it's a ternary number: + is 0, * is 1, 2 is ||
        let operator_count = calibration.equation.len() - 1;
        let max_operator_combo: u32 = 3u32.pow(operator_count as u32) - 1; // operators = 2 -> 3^2 - 1 -> 0t100 - 1 -> 0t22
        'outer: for operator_combo in 0..=max_operator_combo {
            let mut answer = calibration.equation[0];
            for operator_index in 0..operator_count {
                let equation_index = operator_index + 1;
                match get_trit(operator_combo, operator_index) {
                    0 => answer += calibration.equation[equation_index],
                    1 => answer *= calibration.equation[equation_index],
                    2 => answer = concatenate(answer, calibration.equation[equation_index]),
                    _ => unreachable!(),
                }
            }

            if answer == calibration.expected {
                sum += calibration.expected;
                break 'outer;
            }
        }
    }
    println!("{sum}");
}
