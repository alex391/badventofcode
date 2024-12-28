use std::{collections::{HashSet, VecDeque}, fs, mem};

const NEIGHBOR_OFFSETS: [(isize, isize); 4] = [(0, -1), (1, 0), (0, 1), (-1, 0)];

#[derive(Default, Eq, Hash, PartialEq)]
struct Node {
    x: isize, // Storing x and y just for Hash
    y: isize,
    height: u32,
    marked: bool,
    children: Vec<Node>,
}

impl Node {
    
    fn new(height: u32, x: isize, y: isize) -> Self {
        Node {
            height,
            x,
            y,
            marked: false,
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
        let mut child = Node::new(grid[neighbor_y as usize][neighbor_x as usize], neighbor_x, neighbor_y);
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

fn count_reachable_nines(start: Node) -> usize {
    // https://youtu.be/xlVX7dXLS64?si=DhWzmFe13AqBIbOI
    let mut queue: VecDeque<Node> = VecDeque::new();
    let mut counted: HashSet<Node> = HashSet::new();
    queue.push_back(start);

    while !queue.is_empty() {
        let mut v = queue.pop_front().unwrap();
        if !v.marked {
            if v.height == 9 {
                counted.insert(v);
                continue;
            }
            v.marked = true;
            for w in v.children {
                if !w.marked {
                    queue.push_back(w);
                }
            }
        }
    }

    counted.len()

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
                starts.push(Node::new(*height, x as isize, y as isize));
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
