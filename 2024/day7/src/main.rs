use std::{fs, str::FromStr};
struct Calibration {
    test_value: i64,
    equation: Vec<i64>,
}

impl FromStr for Calibration {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut split = s.split(&[' ', ':']).filter(|x| !x.is_empty());
        let test_value: i64 = match split.next().ok_or("Can't parse test value")?.parse() {
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

        Ok(Calibration {
            test_value,
            equation,
        })
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
        let mut max_operator_combo: u32 = 0;

        for i in 0..(calibration.equation.len() - 1) {
            max_operator_combo ^= 1 << i;
        }

        println!("max_operator_combo: :{:#011b}", max_operator_combo);

        for operator_combo in 0..=max_operator_combo {
            let mut answer = calibration.equation[0];
            for i in 1..calibration.equation.len() {
                println!("{:#011b}", operator_combo);
                if is_bit_one(operator_combo, i) {
                    answer *= calibration.equation[i];
                } else {
                    answer += calibration.equation[i];
                }
            }
            if answer == calibration.test_value {
                sum += calibration.test_value;
                break;
            }
        }
    }
    println!("{sum}");
}
