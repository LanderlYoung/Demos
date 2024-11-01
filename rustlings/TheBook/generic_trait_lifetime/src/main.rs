use std::{
    fmt::{format, Display},
    iter::Sum,
};

mod lifetime;

fn main() {
    let number_list = vec![34, 50, 25, 100, 65];

    let result = largest(&number_list);
    println!("The largest number is {result}");

    let char_list = vec!['y', 'm', 'a', 'q'];

    let result = largest(&char_list);
    println!("The largest char is {result}");

    lifetime::test();
}

// ============ Generics ===============
// 1. in struct
struct Point<T> {
    x: T,
    y: T,
}

// 2. in method
impl<T> Point<T> {
    fn x(&self) -> &T {
        &self.x
    }
}

// 模板特化？？？
impl Point<i32> {
    fn x_i32(&self) -> &i32 {
        &self.x
    }
}

// 3. in enum
enum Opt<T> {
    Some(T),
    None,
}

// 4. in function
fn largest<T: PartialOrd>(list: &[T]) -> &T {
    let mut largest = &list[0];

    for item in list {
        if item > largest {
            largest = item
        }
    }

    largest
}

// ============ Trait ==============

trait Summary {
    fn summarize(&self) -> String;

    // default implementation
    fn long_summarize(&self) -> String {
        "Summarize is: ".to_string() + &self.summarize()
    }
}

struct Tweet {
    name: String,
    content: String,
}

impl Summary for Tweet {
    fn summarize(&self) -> String {
        format!("Tweet: {} says {}", self.name, self.content)
    }
}

struct Book {
    author: String,
    toc: String,
}

impl Summary for Book {
    fn summarize(&self) -> String {
        format!("This is book of {}", self.author)
    }
}

// impl Trait 类型实际是个语法糖，会被展开为模板
// 因此这里的Trait方法是编译时静态的，不是OO的多态（虚函数）
fn generate_summary(s: &impl Summary) -> String {
    s.summarize()
}

fn generate_summary2<T: Summary>(s: &T) -> String {
    s.summarize()
}

// 使用 + 指定多个bonds
fn generate_summary3(s: &(impl Summary + Display)) -> String {
    s.summarize()
}

fn generate_summary4<T: Summary + Display>(s: &T) -> String {
    s.summarize()
}

// trait for return type
fn returns_summarizable1() -> impl Summary {
    Tweet {
        name: String::from("horse_ebooks"),
        content: String::from("of course, as you probably already know, people"),
    }
}

// 非多态的方式，不允许if-else返回不同类型
// fn returns_summarizable3(switch: bool) -> impl Summary {
//     if switch {
//         Tweet {
//             name: String::from("horse_ebooks"),
//             content: String::from("of course, as you probably already know, people"),
//         }
//     } else {
//         Book {
//             author: String::from("A"),
//             toc: String::from("empty"),
//         }
//     }
// }

struct TraitMethodButNoImpl {}

impl  TraitMethodButNoImpl {
    fn summarize(&self) -> String {
        return String::from("implemented the trait method, but no explicit impl declare");
    }
}

fn can_we_call() {
    let t = TraitMethodButNoImpl{};
    // error[E0277]: the trait bound `TraitMethodButNoImpl: Summary` is not satisfied
    // generate_summary(&t);
}