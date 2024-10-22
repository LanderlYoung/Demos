use std::io;

fn main() {
    let a: [i32; 5] = [1, 2, 3, 4, 5];
    let first = a[0];
    let a = [1, 2, 3, 4, 5];
    let second = a[1];

    println!("Please enter index!");

    let mut index = String::new();

    io::stdin()
        .read_line(&mut index)
        .expect("read failed");

    let index: usize = index
        .trim()
        .parse()
        .expect("Invalid index");


    println!("index:{index} value:{}, first:{first}, second:{second}", a[index]);
    println!("array is {:?}", a);
}
