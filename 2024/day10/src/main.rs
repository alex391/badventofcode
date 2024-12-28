use std::{fs, mem};

const NEIGHBOR_OFFSETS: [(isize, isize); 4] = [(0, -1), (1, 0), (0, 1), (-1, 0)];

#[derive(Default)]
struct Node {
    height: u32,
    children: Vec<Node>,
}

impl Node {
    fn new(height: u32) -> Self {
        Node {
            height,
            children: Vec::new(),
        }
    }

    fn add_child(&mut self, node: &mut Node) -> bool {
        if node.height != (self.height + 1) {
            return false;
        }
        self.children.push(mem::take(node));
        true
    }
}

fn add_neighbors(grid: &[Vec<u32>], x: usize, y: usize, node: &mut Node) {
    for offset in NEIGHBOR_OFFSETS {
        let neighbor_x: isize = x as isize + offset.0;
        let neighbor_y: isize = y as isize + offset.1;
        if neighbor_y < 0 || neighbor_y as usize >= grid.len() {
            continue;
        }
        if neighbor_x < 0 || neighbor_x as usize >= grid[neighbor_y as usize].len() {
            continue;
        }
        let mut child = Node::new(grid[neighbor_y as usize][neighbor_x as usize]);
        let added = node.add_child(&mut child);
        drop(child); // Dropping here because this node just has default values so we probably don't want to use it later
        if !added {
            continue;
        }
        let child_index = node.children.len() - 1;
        add_neighbors(
            grid,
            neighbor_x as usize,
            neighbor_y as usize,
            &mut node.children[child_index],
        );
    }
}

fn count_reachable_nines(start: Node) -> i32 {
    // TODO: this over-counts, bfs?
    if start.height == 9 {
        return 1;
    }
    let mut sum = 0;
    for child in start.children {
        sum += count_reachable_nines(child);
    }
    sum
}

fn main() {
    let contents = fs::read_to_string("input.txt").expect("Can't read input.txt!");
    let grid: Vec<Vec<u32>> = contents
        .lines()
        .map(|line| line.chars().map(|c| c.to_digit(10).unwrap()).collect())
        .collect();
    let mut starts: Vec<Node> = Vec::new();

    for (y, row) in grid.iter().enumerate() {
        for (x, height) in row.iter().enumerate() {
            if *height == 0 {
                starts.push(Node::new(*height));
                let start_index = starts.len() - 1;
                add_neighbors(grid.as_slice(), x, y, &mut starts[start_index]);
            }
        }
    }

    let mut score_sum = 0;
    for start in starts {
        score_sum += count_reachable_nines(start);
    }
    println!("{score_sum}")


}
