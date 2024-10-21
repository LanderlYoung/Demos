use std::io::{stdin, Write};
use std::cmp::Ordering;
use rand::Rng;

fn main() {
    println!("Guess the number!");
    let secret_number = rand::thread_rng().gen_range(1..=100);

    loop {
        print!("Please input your number: ");
        std::io::stdout().flush().expect("fuck");

        let mut guess = String::new();
        stdin().read_line(&mut guess)
            .expect("Fail to read from stdin");

        let guess: u32 = match guess.trim().parse() {
            Ok(num) => num,
            Err(_) => continue,
        };

        match guess.cmp(&secret_number) {
            Ordering::Less => println!("Too small!"),
            Ordering::Greater => println!("Too big!"),
            Ordering::Equal => {
                println!("You win!");
                break;
            }
        };
    }
}
