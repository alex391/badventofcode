use std::{any::Any, fs};

#[derive(PartialEq, Eq)]
struct Point {
    x: usize,
    y: usize,
}

impl Point {
    fn new() -> Self {
        Point { x: 0, y: 0 }
    }
}

#[derive(PartialEq, Eq)]
struct Offset {
    x: isize,
    y: isize,
}

impl Offset {
    /// Get Some(Point) that is offset by this offset, or None either would be
    /// negative or if it would overflow isize
    fn add(self, p: &Point) -> Option<Point> {
        let x: isize = self.x + isize::try_from(p.x).ok()?;
        let y: isize = self.y + isize::try_from(p.y).ok()?;

        Some(Point {
            x: usize::try_from(x).ok()?,
            y: usize::try_from(y).ok()?,
        })
    }

    fn new(x: isize, y: isize) -> Self {
        Offset {x, y}
    }
}

#[derive(Clone, Copy)]
enum Directions {
    Up,
    Right,
    Down,
    Left
}

impl Directions {
    fn turn(&mut self) {
        *self = match *self {
            Directions::Up => Self::Right,
            Directions::Right => Self::Down,
            Directions::Down => Self::Left,
            Directions::Left => Self::Up,
        };
    }

    fn look_offset(self) -> Offset {
        match self {
            Directions::Up => Offset::new(0, -1),
            Directions::Right => Offset::new(1, 0),
            Directions::Down => Offset::new(0, 1),
            Directions::Left => Offset::new(-1, 0),
        }
    }
}


fn main() {
    let contents =
        fs::read_to_string("input.txt").expect("Should have been able to read input.txt!");
    let map: Vec<Vec<char>> = contents.lines().map(|l| l.chars().collect()).collect();

    let mut guard = Point::new();

    'outer: for (y, line) in (&map).into_iter().enumerate() {
        for (x, char) in line.into_iter().enumerate() {
            if *char == '^' {
                guard.x = x;
                guard.y = y;
                break 'outer;
            }
        }
    }

    let mut visited = 0;

    let mut direction: Directions = Directions::Up;
    loop {
        let to_look: Point = match direction.look_offset().add(&guard) {
            Some(p) => p,
            None => break,
        };
        
        let is_obstacle: bool = match (match map.get(to_look.y) {
            Some(row) => row,
            None => break
        }.get(to_look.x)) {
            Some(c) => match c {
                '#' => true,
                '.' => false,
                _ => unreachable!()
            },
            None => break,
        };
        if is_obstacle {
            direction.turn();
        } else {
            guard = to_look;
            visited += 1;
        }
    }
        

    println!("{visited}")
}
