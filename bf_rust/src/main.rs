use std::{char, env, fs::File, io::stdin, io::Read, process};

const INCREMENT: char = '+';
const DECREMENT: char = '-';
const RIGHT: char = '>';
const LEFT: char = '<';
const LOOP_START: char = '[';
const LOOP_END: char = ']';
const OUTPUT: char = '.';
const INPUT: char = ',';

const MEMORY_SIZE: usize = 1024;

fn main() {
    let args: Vec<String> = env::args().collect();

    if args.len() < 2 {
        eprintln!("Error: A Brainfuck code is not passed as a command-line argument.");
        eprintln!("Please pass an argument as the form, $ brainfuck [FILENAME].");
        process::exit(-1);
    }

    let mut code = String::new();
    let mut f: File =
        File::open(&args[1]).expect(&format!("Error: The file {}, cannot be opened.", &args[1]));
    let _ = f.read_to_string(&mut code);

    let mut memory: Vec<u8> = vec![0; MEMORY_SIZE];
    let mut stack: Vec<usize> = Vec::new();

    let mut ptr = 0;
    let mut code_ptr = 0;
    let code_len = code.chars().count();

    while code_ptr < code_len {
        match code.chars().nth(code_ptr) {
            Some(INCREMENT) => {
                memory[ptr] = if memory[ptr] < u8::MAX {
                    memory[ptr] + 1
                } else {
                    // void overflow
                    0
                }
            }
            Some(DECREMENT) => {
                memory[ptr] = if memory[ptr] > u8::MIN {
                    memory[ptr] - 1
                } else {
                    // avoid underflow
                    255
                }
            }
            Some(RIGHT) => ptr = if ptr >= MEMORY_SIZE - 1 { 0 } else { ptr + 1 },
            Some(LEFT) => ptr = if ptr <= 0 { MEMORY_SIZE - 1 } else { ptr - 1 },
            Some(LOOP_START) => {
                stack.push(code_ptr);
                if memory[ptr] == 0 {
                    let mut depth = 1;

                    while depth > 0 {
                        code_ptr += 1;

                        if code_ptr >= code_len {
                            eprintln!("Error: Cannot find \"]\".");
                            process::exit(-1);
                        }

                        match code.chars().nth(code_ptr) {
                            Some(LOOP_START) => depth += 1,
                            Some(LOOP_END) => depth -= 1,
                            Some(_) => {}
                            None => {}
                        }
                    }

                    stack.pop();
                }
            }
            Some(LOOP_END) => {
                if stack.is_empty() {
                    eprintln!("Error: Loop start order {} is not found.", LOOP_START);
                    process::exit(-1);
                }
                code_ptr = stack.pop().unwrap() - 1;
            }
            Some(OUTPUT) => print!("{}", memory[ptr] as char),
            Some(INPUT) => {
                let mut s = String::new();
                let _ = stdin().read_line(&mut s).unwrap();
                memory[ptr] = s.trim().chars().nth(0).unwrap() as u8;
            }
            Some(_) => {}
            None => {}
        }
        code_ptr += 1;
    }
}
