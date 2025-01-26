use std::convert::Infallible;
use std::hash::Hash;
use std::{
    collections::{BinaryHeap, HashMap},
    fs,
    str::FromStr,
};
use text_io::scan;

const A_TOKENS: i32 = 3;
const B_TOKENS: i32 = 1;

#[derive(Debug)]
struct ClawMachine {
    /*
    https://www.desmos.com/calculator/pnzvxzaq1x
    Button A: X+n, Y+p
    Button B: X+m, Y+q
    Prize: X=o, Y=r
     */
    n: f64, // Yeah this naming scheme is terrible... see repo name
    m: f64,
    o: f64,
    p: f64,
    q: f64,
    r: f64,
}

impl FromStr for ClawMachine {
    type Err = Infallible;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let n: f64;
        let m: f64;
        let o: f64;
        let p: f64;
        let q: f64;
        let r: f64;


        scan!(s.bytes() => "Button A: X+{}, Y+{}\nButton B: X+{}, Y+{}\nPrize: X={}, Y={}", n, p, m, q, o, r);
        Ok(Self { n, m, o, p, q, r })
    }
}


fn main() {
    let contents =
        fs::read_to_string("input.txt").expect("Should have been able to read input.txt");
    let contents = contents.split("\n\n");
    let mut cost = 0;
    for item in contents {
        let claw_machine: ClawMachine = item.parse().unwrap();
        if let Some(c) = shortest_path_cost(claw_machine) {
            cost += c
        }
    }
    println!("{cost}");
}
