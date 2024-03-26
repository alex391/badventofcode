use std::{collections::VecDeque, fs};

fn main() {
    let contents = fs::read_to_string("input.txt").expect("Should have been able to read the file");
    let histories = contents.lines().map(|x| x.split(' ').filter_map(|x| x.parse::<i32>().ok()));


    let mut count = 0;
    for history in histories {
        let mut differences: Vec<VecDeque<i32>> = Vec::new();
        differences.push(history.collect());
        loop {
            let last: &VecDeque<i32> = differences.last().expect("Predictions is empty!");
            let mut difference: VecDeque<i32> = VecDeque::new();
            for (i, x) in last.iter().enumerate() {
                if i + 1 < last.len() {
                    difference.push_back(last[i + 1] - x)
                }
            }
            differences.push(difference);
            if all_zero(differences.last().unwrap()) {
                break;
            }
        }
        count += extrapolate_left(differences);
    }
    println!("{count}");
}

/// Return true if the VecDeque is all zero
fn all_zero(x: &VecDeque<i32>) -> bool {
    for i in x {
        if *i != 0 {
            return false;
        }
    }
    true
}



fn extrapolate_left(differences: Vec<VecDeque<i32>>) -> i32 {
    extrapolate_worker_left_side(differences)[0][0]
}


fn extrapolate_worker_left_side(predictions: Vec<VecDeque<i32>>) -> Vec<VecDeque<i32>> {
    if predictions.len() == 1 {
        return predictions;
    }
    let last = predictions.last().unwrap();
    let mut second_to_last = predictions[predictions.len() - 2].clone();
    second_to_last.push_front(second_to_last[0] - last[0]);

    extrapolate_worker_left_side([&predictions[0..predictions.len() - 2], &[second_to_last]].concat())
}