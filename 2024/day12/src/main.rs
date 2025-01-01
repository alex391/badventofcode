use std::{
    collections::{HashSet, VecDeque},
    fs, os::unix::raw::pid_t,
};

const NEIGHBOR_OFFSETS: [IPoint; 4] = [IPoint(0, -1), IPoint(1, 0), IPoint(0, 1), IPoint(-1, 0)];
const CORNER_OFFSETS: [IPoint; 4] = [IPoint(1, 1), IPoint(1, -1), IPoint(-1, 1), IPoint(-1, -1)]; // https://advent-of-code.xavd.id/writeups/2024/day/12/

#[derive(Debug)]
struct Price {
    area: i32,
    sides: i32,
}

impl Price {
    fn get_price(&self) -> i32 {
        self.area * self.sides
    }
}

#[derive(Hash, Eq, PartialEq, Clone, Copy)]
struct UPoint(usize, usize);

impl UPoint {
    fn to_ipoint(self) -> Option<IPoint> {
        let zero = isize::try_from(self.0);
        let one = isize::try_from(self.1);

        if let Ok(zero) = zero {
            if let Ok(one) = one {
                return Some(IPoint(zero, one));
            }
        }
        // This None case is ridiculous: .0 and .1 are <= 140, and, regardless, array indexes are required to be <= isize::MAX.
        None
    }
}

#[derive(Hash, Eq, PartialEq, Clone, Copy)]
struct IPoint(isize, isize);

impl IPoint {
    fn to_upoint(self) -> Option<UPoint> {
        let zero = usize::try_from(self.0);
        let one = usize::try_from(self.1);

        if let Ok(zero) = zero {
            if let Ok(one) = one {
                return Some(UPoint(zero, one));
            }
        }
        None // Unlike UPoint's to_ipoint, this function is likely to return None
    }

    fn add(&self, rhs: &IPoint) -> IPoint {
        IPoint(self.0 + rhs.0, self.1 + rhs.1)
    }

    /// Are both .0 and .1 < max and >= 0?
    fn in_bounds(&self, max: usize) -> bool {
        if max > isize::MAX as usize {
            return self.0 >= 0 && self.1 > 0;
        }
        let max = max as isize;
        self.0 < max && self.0 >= 0 && self.1 < max && self.1 >= 0
    }
}

/// Return the number of corners that are here
fn detect_corners(masked_garden: &[Vec<bool>], point: IPoint, interior: bool) -> i32 {
    // Thanks to https://advent-of-code.xavd.id/writeups/2024/day/12/
    let mut count = 0;
    for i in 0..NEIGHBOR_OFFSETS.len() {
        let first_offset = NEIGHBOR_OFFSETS[i];
        let second_offset = NEIGHBOR_OFFSETS[(i + 1) % NEIGHBOR_OFFSETS.len()];
        let first_point = point.add(&first_offset);
        let second_point = point.add(&second_offset);
        let mut first: bool = !interior;
        let mut second: bool = !interior;
        if first_point.in_bounds(masked_garden.len()) {
            let first_point = first_point.to_upoint().unwrap();
            first = masked_garden[first_point.1][first_point.0];
        }
        if second_point.in_bounds(masked_garden.len()) {
            let second_point = second_point.to_upoint().unwrap();
            second = masked_garden[second_point.1][second_point.0];
        }
        if first == interior && second == interior {
            count += 1;
            if interior {
                continue;
            }
        }
    }
    count
}

fn detect_all_corners(masked_garden: &[Vec<bool>], point: UPoint) -> i32 {
    let mut corner_count = 0;
    let signed_point = point.to_ipoint().unwrap();
    // Exterior corners
    for offset in CORNER_OFFSETS {
        corner_count += detect_corners(masked_garden, signed_point.add(&offset), false);
    }
    //  Interior corners
    for offset in CORNER_OFFSETS {
        corner_count += detect_corners(masked_garden, signed_point.add(&offset), true);
    }
    corner_count 
}

fn flood_fill(masked_garden: &mut [Vec<bool>], start: UPoint) -> Price {
    // https://youtu.be/xlVX7dXLS64
    let mut queue: VecDeque<UPoint> = VecDeque::new();
    let mut visited: HashSet<UPoint> = HashSet::new();
    let mut price: Price = Price { area: 0, sides: 0 };
    queue.push_back(start);
    while !queue.is_empty() {
        let v = queue.pop_front().unwrap();
        if !visited.contains(&v) {
            price.area += 1;
            visited.insert(v);
            price.sides += detect_all_corners(masked_garden, v);
            for offset in NEIGHBOR_OFFSETS {
                let v = v.to_ipoint().unwrap();
                let neighbor = v.add(&offset);

                let mut interior: bool = false;
                let unsigned_neighbor = neighbor.to_upoint();
                if let Some(unsigned_neighbor) = unsigned_neighbor {
                    if let Some(row) = masked_garden.get(unsigned_neighbor.1) {
                        if let Some(i) = row.get(unsigned_neighbor.0) {
                            interior = *i;
                        }
                    }
                }


                queue.push_back(match unsigned_neighbor {
                    Some(x) => {
                        if interior {
                            x
                        } else {
                            continue;
                        }
                    }
                    None => continue,
                });
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
                    let price = flood_fill(&mut masked_garden, UPoint(x, y));
                    price_total += price.get_price();
                    println!("{region_name}: {:?}", price);
                }
            }
        }
    }
    println!("{price_total}")
}
