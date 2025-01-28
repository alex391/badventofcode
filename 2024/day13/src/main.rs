#![feature(f128)]
use std::convert::Infallible;
use std::{fs, str::FromStr};
use text_io::scan;


const A_TOKENS: i64 = 3;
const B_TOKENS: i64 = 1;

const PART_ONE: bool = false;

type Intersection = Option<(f128, f128)>;

// https://stackoverflow.com/a/31497642
trait IntersectionExt {
    #[allow(clippy::too_many_arguments)] // annoying to unpack
    fn new(x1: f128, y1: f128, x2: f128, y2: f128, x3: f128, y3: f128, x4: f128, y4: f128) -> Self;
}

impl IntersectionExt for Intersection {
    fn new(x1: f128, y1: f128, x2: f128, y2: f128, x3: f128, y3: f128, x4: f128, y4: f128) -> Self {
    // Thanks https://stackoverflow.com/a/385355
        let x12 = x1 - x2;
        let x34 = x3 - x4;
        let y12 = y1 - y2;
        let y34 = y3 - y4;

        let c = x12 * y34 - y12 * x34;

        if c.abs() < 0.01 {
            None
        }
        else {
            let a = x1 * y2 - y1 * x2;
            let b = x3 * y4 - y3 * x4;

            let x = (a * x34 - b * x12) / c;
            let y = (a * y34 - b * y12) / c;

            Some((x, y))
        }
    }


}

fn is_whole(float: f128) -> bool {
    let fraction_part = float.fract().abs();
    fraction_part <= 0.0001 || fraction_part >= 0.9999
}

#[derive(Debug)]
struct ClawMachine {
    /*
    https://www.desmos.com/calculator/pnzvxzaq1x
    Button A: X+n, Y+p
    Button B: X+m, Y+q
    Prize: X=o, Y=r
     */
    n: f128, // Yeah this naming scheme is terrible... see repo name
    m: f128,
    o: f128,
    p: f128,
    q: f128,
    r: f128,
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
        Ok(Self {
            n: n as f128,
            m: m as f128,
            o: (o + if !PART_ONE { 10000000000000.0 } else { 0.0 }) as f128,
            p: p as f128,
            q: q as f128,
            r: (r + if !PART_ONE { 10000000000000.0 } else { 0.0 }) as f128,
        })
    }
}

impl ClawMachine {
    fn cost(&self) -> i64 {
        let intersection = Intersection::new(
            0.0,
            self.f(0.0),
            1.0,
            self.f(1.0),
            0.0,
            self.g(0.0),
            1.0,
            self.g(1.0),
        );
        if let Some(point) = intersection {
            if is_whole(point.0) && is_whole(point.1) {
                return (point.0.round() as i64 * A_TOKENS) + (point.1.round() as i64 * B_TOKENS)
            }
        }
        0
    }

    fn f(&self, x: f128) -> f128 {
        (self.o - (self.n * x)) / self.m
    }

    fn g(&self, x: f128) -> f128 {
        (self.r - (self.p * x)) / self.q
    }
}

fn main() {
    let contents =
        fs::read_to_string("input.txt").expect("Should have been able to read input.txt");
    let contents = contents.split("\n\n");
    let mut cost: i64 = 0;
    for item in contents {
        let claw_machine: ClawMachine = item.parse().unwrap();
        cost += claw_machine.cost();
    }
    println!("{cost}");
}
