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

/// From the right (so is_bit_1(0b01, 0) returns true)
fn is_bit_one(integer: u32, index: usize) -> bool {
    (integer >> index) & 1 == 0
}

fn main() {
    let contents = fs::read_to_string("input.txt").expect("Can't read input.txt!");
    let calibrations: Vec<Calibration> = contents.lines().map(|l| l.parse().unwrap()).collect();

    let mut sum = 0;
    for calibration in calibrations {
        // The idea is that we'll go through all of the combinations as if
        // it's a binary number: + is 0, * is 1
        let operator_count = calibration.equation.len() - 1;
        let max_operator_combo: u32 = 2u32.pow((operator_count).try_into().unwrap()) - 1; // operators = 2 -> 2^2 - 1 -> 0b100 - 1 -> 0b11

        'outer: for operator_combo in 0..=max_operator_combo {
            let mut answer = calibration.equation[0];
            for operator_index in 0..operator_count {
                let equation_index = operator_index + 1;
                match is_bit_one(operator_combo, operator_index) {
                    true => answer *= calibration.equation[equation_index],
                    false => answer += calibration.equation[equation_index],
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
