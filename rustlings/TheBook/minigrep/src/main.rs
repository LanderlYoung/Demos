mod minigrep;

use minigrep::{Config, MiniGrep};
use std::env::args;
use std::error;

fn main() -> Result<
    (),
    // what the heck is this???
    Box<dyn error::Error>,
> {
    let a: Vec<String> = args().collect();

    let config = Config::new(&a)?;
    let mut minigrep = MiniGrep::new(&config)?;

    while let Some(line) = minigrep.next() {
        println!("{}", line);
    }

    Ok(())
}
