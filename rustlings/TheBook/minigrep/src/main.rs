use minigrep::{Config, MiniGrep};
use std::env::args;
use std::error;
use regex::Regex;
use utf16_literal;

// #![feature(proc_macro_hygiene)]
// extern crate utf16_literal;

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

    tegex_test();

    Ok(())
}

fn tegex_test() {
    let haystack = "samwise";
    let re = Regex::new(r"samwise|sam").unwrap();
    assert_eq!("samwise", re.find(haystack).unwrap().as_str());
}

fn u16string_test() {
    let _u16string: &[u16; 6] = utf16_literal::utf16!("Foo\u{1234}😀");

}