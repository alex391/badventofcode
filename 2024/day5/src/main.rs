use std::{cmp::Ordering, fmt::Display, fs, num::ParseIntError, str::FromStr};

#[derive(PartialEq, Eq)]
struct PageOrder {
    a: i32,
    b: i32,
}

impl FromStr for PageOrder {
    type Err = String;

    fn from_str(s: &str) -> Result<Self, Self::Err> {
        let mut split = s.split('|');
        let before: Result<i32, ParseIntError> = split.next().ok_or("Can't parse before!")?.parse();
        let before: i32 = match before {
            Ok(i) => i,
            Err(e) => return Err(e.to_string()),
        };

        let after: Result<i32, ParseIntError> = split.next().ok_or("Can't parse after!")?.parse();
        let after: i32 = match after {
            Ok(i) => i,
            Err(e) => return Err(e.to_string()),
        };

        Ok(PageOrder { a: before, b: after })
    }
}

impl Display for PageOrder {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}|{}", self.a, self.b)
    }
}

fn page_order_cmp(a: i32, b: i32, page_orders: &[PageOrder]) -> std::cmp::Ordering {
    let comparing = PageOrder {a, b};
    let comparing_swapped = PageOrder {b, a};
    for page_order in page_orders {
        if comparing == *page_order {
            return Ordering::Less;
        }
        if comparing_swapped == *page_order {
            return Ordering::Greater;
        }
    }
    Ordering::Equal
}

fn main() {
    let contents =
        fs::read_to_string("input.txt").expect("Should have been able to read input.txt!");
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
        let update: Vec<i32> = line.split(',').map(|x| x.parse().unwrap()).collect();
        updates.push(update);
    }

    let mut sum = 0;

    for mut update in updates {
        let old_update = update.clone();
        update.sort_unstable_by(|a, b| page_order_cmp(*a, *b, &page_orders));
        if old_update != update {
            let middle_index = update.len() / 2;
            sum += update[middle_index];
        }
    }
    println!("{sum}");

}
