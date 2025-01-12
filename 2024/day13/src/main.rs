use std::{
    collections::{BinaryHeap, HashMap},
    fs,
    str::FromStr,
};
use text_io::scan;
use std::hash::Hash;

const A_TOKENS: i32 = 3;
const B_TOKENS: i32 = 1;


#[derive(Debug)]
struct ClawMachine {
    a: (i32, i32),
    b: (i32, i32),
    prize: (i32, i32),
}

impl FromStr for ClawMachine {
    type Err = ();

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut a: (i32, i32) = Default::default();
        let mut b: (i32, i32) = Default::default();
        let mut prize: (i32, i32) = Default::default();
        scan!(s.bytes() => "Button A: X+{}, Y+{}\nButton B: X+{}, Y+{}\nPrize: X={}, Y={}", a.0, a.1, b.0, b.1, prize.0, prize.1);
        Ok(Self { a, b, prize })
    }
}


#[derive(Clone, Copy)]
struct Node {
    coordinate: (i32, i32),
    cost: i32,
}

impl PartialEq for Node{
    fn eq(&self, other: &Self) -> bool {
        self.coordinate == other.coordinate
    }
}

impl Eq for Node {
    
}

impl Hash for Node {
    fn hash<H: std::hash::Hasher>(&self, state: &mut H) {
        self.coordinate.hash(state);
    }
}

impl PartialOrd for Node {
    fn partial_cmp(&self, other: &Self) -> Option<std::cmp::Ordering> {
        Some(self.cmp(other))
    }
}

impl Ord for Node {
    fn cmp(&self, other: &Self) -> std::cmp::Ordering {
        match self.cost.cmp(&other.cost) {
            // Used only in min heap
            std::cmp::Ordering::Less => std::cmp::Ordering::Greater,
            std::cmp::Ordering::Equal => std::cmp::Ordering::Equal,
            std::cmp::Ordering::Greater => std::cmp::Ordering::Less,
        }
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

/// Do this:
/// Mash A until you just undershoot, record the cost
/// Mash B until you just undershoot, record the cost
/// return whichever cost is smaller
fn h(claw_machine: &ClawMachine, start: Node) -> i32 {
    let start = start.coordinate;
    let mut a_cost = 0;
    let mut a_position = start;
    while a_position.0 < claw_machine.prize.0 && a_position.1 < claw_machine.prize.1 {
        a_position.0 += claw_machine.a.0;
        a_position.1 += claw_machine.a.1;
        a_cost += A_TOKENS;
    }

    let mut b_cost = 0;
    let mut b_position = start;
    while b_position.0 < claw_machine.prize.0 && b_position.1 < claw_machine.prize.1 {
        b_position.0 += claw_machine.b.0;
        b_position.1 += claw_machine.b.1;
        b_cost += B_TOKENS;
    }
    if a_cost < b_cost {
        a_cost
    } else {
        b_cost
    }
}

fn shortest_path_cost(claw_machine: ClawMachine) -> Option<i32> {
    // https://en.wikipedia.org/wiki/A*_search_algorithm
    // Need to use something like A* because there are potentially 2^100 nodes
    let start = Node{ coordinate: (0, 0), cost: 0};
    let mut open_set: BinaryHeap<Node> = BinaryHeap::new();
    open_set.push(start);

    // Default to Infinity
    let mut g_score: HashMap<Node, i32> = HashMap::new();
    g_score.insert(start, 0);

    // Default to Infinity
    let mut f_score: HashMap<Node, i32> = HashMap::new();
    f_score.insert(start, h(&claw_machine, start));

    while !open_set.is_empty() {
        let current = open_set.pop().unwrap();
        if current.coordinate == claw_machine.prize {
            return Some(current.cost);
        }
        {
            let mut neighbor = Node{coordinate: (current.coordinate.0 + claw_machine.a.0, current.coordinate.1 + claw_machine.a.1), cost: 0};
            let tentative_g_score = g_score.get(&current).unwrap_or(&i32::MAX).saturating_add(A_TOKENS);
            if tentative_g_score < *g_score.get(&neighbor).unwrap_or(&i32::MAX) {
                g_score.insert(neighbor, tentative_g_score);
                f_score.insert(neighbor, tentative_g_score + h(&claw_machine, neighbor));
                neighbor.cost = tentative_g_score;
                open_set.push(neighbor);
            }
        }

        {
            let mut neighbor = Node{coordinate: (current.coordinate.0 + claw_machine.b.0, current.coordinate.1 + claw_machine.b.1), cost: 0};
            let tentative_g_score = g_score.get(&current).unwrap_or(&i32::MAX).saturating_add(B_TOKENS);
            if tentative_g_score < *g_score.get(&neighbor).unwrap_or(&i32::MAX) {
                g_score.insert(neighbor, tentative_g_score);
                f_score.insert(neighbor, tentative_g_score + h(&claw_machine, neighbor));
                neighbor.cost = tentative_g_score;
                open_set.push(neighbor);
            }
        }
    }
    None
}
