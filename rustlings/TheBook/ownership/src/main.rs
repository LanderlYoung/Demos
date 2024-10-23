fn main() {
    simple_scope();
    string_assign();
    string_clone_assign();
    int_assign();

    reference();

    string_slice();
}

fn simple_scope() {
    {
        let _string_literal: &str = "Hello World!";
    }
    {
        let mut s = String::from("hello");
        s.push_str(", world!");
        println!("{}", s);
    }
}

fn string_assign() {
    let s1 = String::from("hello");
    let s2 = s1;

    // println!("{s1}, world!");
    // this code won't compile
    // error[E0382]: borrow of moved value: `s1`
    /*
  error[E0382]: borrow of moved value: `s1`
  --> src/main.rs:20:15
   |
17 |     let s1 = String::from("hello");
   |         -- move occurs because `s1` has type `String`, which does not implement the `Copy` trait
18 |     let s2 = s1;
   |              -- value moved here
19 |
20 |     println!("{s1}, world!");
   |               ^^^^ value borrowed here after move
   |
   = note: this error originates in the macro `$crate::format_args_nl` which comes from the expansion of the macro `println` (in Nightly builds, run with -Z macro-backtrace for more info)
help: consider cloning the value if the performance cost is acceptable
   |
18 |     let s2 = s1.clone();
   | 
    */
}

fn string_clone_assign() {
    let s1 = String::from("hello");
    let s2 = s1.clone();
    println!("s2 = {s2}");
}

fn int_assign() {
    let i1: i32 = 0;
    let i2 = i1;
    println!("i1 = {i1}");
}

fn reference() {
    let mut s = String::from("hello");
    let len = string_length(&s);
    change(&mut s);

    println!("len: {len} after change: {s}");

}

fn string_length(s: &String) -> usize {
    s.len()
}

fn change(s: &mut String) {
    s.push_str(", world!");
}

fn mutable_reference() {
    let mut s = String::from("hello");
    let r1 = &mut s;
    /*
    let r2 = &mut s;
    let r3 = &s;

    println!("{s} {r1} {r2} {r3}");
    */
}

fn string_slice() {
    let mut s = String::from("hello world");
    let first_word: &str = first_word(&s);

    // error[E0502]: cannot borrow `s` as mutable because it is also borrowed as immutable
    // s.clear();

    println!("first world:{first_word}");

    // other form
}

fn first_word(s: &str) -> &str {
    let bytes = s.as_bytes();

    for (i, &item) in bytes.iter().enumerate() {
        if item == b' ' {
            return &s[..i];
        }
    }

    &s[..]
}