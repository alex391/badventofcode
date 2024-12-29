use std::{
    collections::{HashSet, VecDeque},
    fs,
};

const NEIGHBOR_OFFSETS: [(isize, isize); 4] = [(0, -1), (1, 0), (0, 1), (-1, 0)];

#[derive(Debug)]
struct Price {
    area: i32,
    perimeter: i32,
}

impl Price {
    fn get_price(&self) -> i32 {
        self.area * self.perimeter
    }
}

#[derive(Hash, Eq, PartialEq, Clone, Copy)]
struct Point(usize, usize);

//
fn flood_fill(masked_garden: &mut [Vec<bool>], start: Point) -> Price {
    // https://youtu.be/xlVX7dXLS64
    let mut queue: VecDeque<Point> = VecDeque::new();
    let mut visited: HashSet<Point> = HashSet::new();
    let mut price: Price = Price {
        area: 0,
        perimeter: 0,
    };
    queue.push_back(start);
    while !queue.is_empty() {
        let v = queue.pop_front().unwrap();
        if !visited.contains(&v) {
            price.area += 1;
            visited.insert(v);
            for offset in NEIGHBOR_OFFSETS {
                let neighbor_x: usize = match usize::try_from(v.0 as isize + offset.0) {
                    Ok(x) => x,
                    Err(_) => {
                        price.perimeter += 1;
                        continue;
                    }
                };
                let neighbor_y: usize = match usize::try_from(v.1 as isize + offset.1) {
                    Ok(y) => y,
                    Err(_) => {
                        price.perimeter += 1;
                        continue;
                    }
                };
                let neighbor_row = match masked_garden.get(neighbor_y) {
                    Some(row) => row,
                    None => {
                        price.perimeter += 1;
                        continue;
                    }
                };
                let neighbor: bool = match neighbor_row.get(neighbor_x) {
                    Some(n) => *n,
                    None => {
                        price.perimeter += 1;
                        continue;
                    }
                };
                if neighbor {
                    queue.push_back(Point(neighbor_x, neighbor_y));
                } else {
                    price.perimeter += 1;
                }
            }
        }
    }
    for point in visited {
        masked_garden[point.1][point.0] = false;
    }
    price
}

fn main() {
    let contents = fs::read_to_string("input.txt").expect("Can't read input.txt!");
    let garden: Vec<Vec<char>> = contents.lines().map(|l| l.chars().collect()).collect();

    let mut region_names: HashSet<char> = HashSet::new();
    for row in &garden {
        for plot in row {
            region_names.insert(*plot);
        }
    }

    let mut price_total = 0;

    for region_name in region_names {
        let mut masked_garden: Vec<Vec<bool>> = garden
            .clone()
            .into_iter()
            .map(|row| row.into_iter().map(|c| c == region_name).collect())
            .collect();
        for y in 0..masked_garden.len() {
            for x in 0..masked_garden[y].len() {
                if masked_garden[y][x] {
                    let price = flood_fill(&mut masked_garden, Point(x, y));
                    price_total += price.get_price();
                    // println!("{:?}", price);
                }
            }
        }
    }
    println!("{price_total}")
}
