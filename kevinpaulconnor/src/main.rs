use std::env;
mod set1;

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() > 1 {
        let challenge = &args[1];
        if challenge == "1_3" {
            set1::solve_1_3();
        }
    }
}