use std::{fmt::Display, fs, num::ParseIntError, str::FromStr};

struct PageOrder {
    before: i32,
    after: i32
}

impl FromStr for PageOrder {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut split = s.split('|');
        let before: Result<i32, ParseIntError> = split.next().ok_or("Can't parse before!")?.parse();
        let before: i32 = match before {
            Ok(i) => i,
            Err(e) => return Err(e.to_string())
        };

        let after: Result<i32, ParseIntError> = split.next().ok_or("Can't parse after!")?.parse();
        let after: i32 = match after {
            Ok(i) => i,
            Err(e) => return Err(e.to_string())
        };

        Ok(PageOrder {before, after})
    }
}

impl Display for PageOrder {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}|{}", self.before, self.after)
    }
}

fn main() {
    let contents = fs::read_to_string("input.txt").expect("Should have been able to read the file");
    let mut contents = contents.lines();

    let mut page_orders: Vec<PageOrder> = Vec::new();

    loop {
        let line = contents.next();
        let line = match line {
            Some(line) => line,
            None => break,
        };
        if line.is_empty() {
            break; // Done with PageOrders
        }
        page_orders.push(line.parse().unwrap());

    }

    let mut updates: Vec<Vec<i32>> = Vec::new();

    for line in contents {
        let update: Vec<i32> = line.split(',').map(|x| {x.parse().unwrap()}).collect();
        updates.push(update);
    }
    
    for page_order in page_orders {
        println!("{page_order}")
    }
    println!();
    for update in updates {
        println!("{:?}", update)
    }
}
