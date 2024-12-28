use std::fs;

fn digits(n: u64) -> u32 {
    // https://stackoverflow.com/q/52276687
    n.ilog10() + 1
}

fn split_number(n: u64) -> (u64, u64) {
    // Doing this the naive way unless it has performance problems
    let digits = digits(n);
    let string = n.to_string();
    let left: u64 = string[0..((digits / 2) as usize)].parse().unwrap();
    let right: u64 = string[((digits / 2) as usize)..].parse().unwrap();
    (left, right)

}

fn main() {
    let contents = fs::read_to_string("input.txt").expect("Can't read input.txt!");
    let contents = contents.lines().next().expect("File is empty!");

    let mut stones: Vec<u64> = contents.split(' ').map(|x| x.parse().unwrap()).collect();
    for _ in 0..75 {
        let mut new_stones: Vec<u64> = Vec::new();
        for stone in &stones {
            if *stone == 0 {
                new_stones.push(1);
            } else if digits(*stone) % 2 == 0 {
                let (left, right) = split_number(*stone);
                new_stones.push(left);
                new_stones.push(right);
            } else {
                new_stones.push(*stone * 2024)
            }
        }
        stones = new_stones;
    }
    println!("{}", stones.len())
}
