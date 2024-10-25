use std::{collections::HashMap, ops::IndexMut};

fn main() {
    println!("Hello, world!");

    test_vector();
    test_string();
    test_hash_map();
}

#[allow(unused)]
fn test_vector() {
    let mut v: Vec<i32> = vec![1, 2, 3, 4, 5];

    // Q:这里的&很奇怪？看着像是取了一个临时变量的引用？？
    let third = v.index_mut(2); // same as:
    let third: &mut i32 = &mut v[2];
    println!("The third element is {third}");
    *third = 33; // 实验证明还真不是临时变量!!!
    println!("The third element is {}", v[2]);

    // Q: 为什么 `*third = 33;` 需要 *， 这里却不需要？？？
    v[4] = 44;

    let third: Option<&i32> = v.get(20);
    match third {
        Some(third) => println!("v.get == {third}"),
        None => println!("v.get == None"),
    }

    for (index, value) in v.iter().enumerate() {
        println!("v[{index}] == {value}")
    }

    for ref value in v {
        print!("{value} ")
    }
    println!();
}

#[allow(unused)]
fn test_string() {
    let mut s1 = String::from("foo");
    let s2 = "bar";
    s1.push_str(&s2);
    println!("after push {s1} {s2}");

    let s1 = String::from("hello, ");
    let s2 = String::from("world");
    // fn add(mut self, other: &str) -> String
    // s1 is moved, s2 is &
    let s3 = s1 + &s2;
    // s1.len(); // borrow of moved value: `s1`

    let s1 = String::from("hello");
    // let h = s1[0]; // the type `str` cannot be indexed by `{integer}`

    let hello = "Здравствуйте";
    let s = &hello[0..4];
    println!("string slice {s}");

    // thread 'main' panicked at src/main.rs:51:19:
    // byte index 1 is not a char boundary; it is inside 'З' (bytes 0..2) of `Здравствуйте`
    // let s = &hello[0..1];

    print!("for in chars: ");
    for c in "Здравствуйте".chars() {
        print!("{c}")
    }
    println!();

    print!("for in bytes: ");
    for c in "Здравствуйте".bytes() {
        print!("{c} ")
    }
    println!();
}

fn test_hash_map() {
    let mut scores = HashMap::new();
    // rust 的类型推导有点牛逼了，上面这行代码是不和法的，但是有了下面这样就可以推导出来里
    scores.insert(String::from("Blue"), 10);
    scores.insert(String::from("Yellow"), 50);

    let v = scores.get(&String::from("Blue")).copied().unwrap_or(10);
    println!("HashMap: {scores:?} get-value: {v}");

    for (key, value) in &scores {
        println!("key:{key} value:{value}");
    }

    let c = scores.entry(String::from("Red")).or_insert(0);
    *c += 1;
}
