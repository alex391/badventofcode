use std::{fs, collections::VecDeque};

fn main() {
    let contents = fs::read_to_string("input.txt")
        .expect("Should have been able to read the file");
    let mut contents = contents.lines();

    // Parse the input
    const STACK_COUNT: usize = 9; // not worth trying to read this from the file

    let mut stacks: [VecDeque<char>; STACK_COUNT] = Default::default();
    
    // skip the first character in a line, read the next, then skip 4
    // repeat until the number of characters read is equal to STACK_COUNT
    // ignore spaces
    // repeat until contents doesn't start with a '['

    loop {
        let line = contents.next().unwrap();
        if !line.contains('[') {
            break;
        }
        let char_vec: Vec<(usize, char)> = line.chars()
            .skip(1)
            .enumerate()
            .filter(|(index, _)| {
                index % 4 == 0 
            })
            .collect();

        for i in 0..STACK_COUNT {
            let c = char_vec[i].1;
            if c != ' ' {
                stacks[i].push_front(c);
            }
        }
    }
    
    // Then, parse the instructions

    let mut instructions: Vec<Vec<usize>> = Vec::new();
    for line in contents.skip(1) { // skip the newline
        instructions.push(line.split(' ')
            .filter_map(|string| {
                string.parse().ok()
            })
            .collect()
        );
    }
    
    // Then, do the instructions on the stacks

    for instruction in instructions {
        // pop from instruction[1]
        // push to instruction[2]
        // off by 1 - the stacks are 1-indexed
        let stack = &mut stacks[instruction[1] - 1];
        let containers: &mut VecDeque<char> = &mut stack.split_off(stack.len() - instruction[0]);

        stacks[instruction[2] - 1].append(containers);
    }

    for mut stack in stacks {
        print!("{}", stack.pop_front().unwrap());
    }
    println!();
}
