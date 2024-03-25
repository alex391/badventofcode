use std::fs;

fn main() {
    let contents = fs::read_to_string("input.txt").expect("Should have been able to read the file");
    let histories = contents.lines().map(|x| x.split(' ').filter_map(|x| x.parse::<i32>().ok()));

    for history in histories {
        let mut differences: Vec<Vec<i32>> = Vec::new();
        differences.push(history.collect());
        loop {
            let last: &Vec<i32> = differences.last().expect("Predictions is empty!");
            let mut difference: Vec<i32> = Vec::new();
            for (i, x) in last.iter().enumerate() {
                difference.push(last[i + 1] - x)
            }
            differences.push(difference);
            if all_zero(differences.last().unwrap()) {
                break;
            }
        }
    }
}

/// Return true if the slice is all zero
fn all_zero(x: &[i32]) -> bool {
    for i in x {
        if *i != 0 {
            return false;
        }
    }
    true
}

fn extrapolate(predictions: Vec<Vec<i32>>) -> Vec<Vec<i32>> {
    todo!()
}