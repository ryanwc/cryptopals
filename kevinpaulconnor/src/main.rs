use std::env;
use std::fs;
mod set1;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() > 1 {
        let challenge = &args[1];
        if challenge == "1_3" {
            set1::solve_1_3();
        }
        if challenge == "1_4" {
            let filename = &args[2];
            let contents = fs::read_to_string(filename)
                .expect("Something went wrong reading the file");
            set1::solve_1_4(contents);
        }
    }
}