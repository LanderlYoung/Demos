use std::alloc::LayoutErr;

struct User {
    active: bool,
    username: String,
    email: String,
    sign_in_count: u64,
}

fn main() {
    basic_struct();

    rectangle1();
    rectangle2();
    rectangle3();
    rectangle_method();
}

fn basic_struct() {
    // struct init syntax
    let user1 = User {
        active: true,
        username: String::from("tom"),
        email: String::from("tom@xx.com"),
        sign_in_count: 1,
    };

    let User { // struct destruct assign
        username,
        email,
        active,
        ..
    } = &user1;
    println!("destructing from user1 {username} {email} {active}");

    let tuple: (u32, String) = (10, String::from("hello"));
    // tuple destruct assign
    let (int, string) = tuple;

    let username = String::from("jerry");
    let email = String::from("jerry@xx.com");

    let user2 = User {
        // Field Init Shorthand
        // field and variable have the same name
        username,
        email,
        // Struct Update Syntax
        ..user1 // just like =, it moves data
    };

    println!("u2.name:{} u2.active:{}", user2.username, user2.active);
    println!("u1.name:{} u1.active:{}", user1.username, user1.active);

    let user3 = User { ..user1 };

    println!("u3.name:{} u3.active:{}", user3.username, user3.active);
    println!("u1.active:{}", user1.active); // user1.active is copyed to user3
                                            // user1.username/email are move to user3, can not be used
                                            // println!("u1.name:{}", user1.username);
}

fn rectangle1() {
    fn area(width: u32, height: u32) -> u32 {
        width * height
    }

    println!("rectangle1: {}", area(30, 50));
}

fn rectangle2() {
    fn area(rect: &(u32, u32)) -> u32 {
        rect.0 * rect.1
    }

    let rect = (30, 50);

    println!("rectangle2: {:?} {}", rect, area(&rect));
}

#[derive(Debug)]
struct Rectangle {
    width: u32,
    height: u32,
}

fn rectangle3() {
    fn area(rect: &Rectangle) -> u32 {
        rect.width * rect.height
    }

    let rect = Rectangle {
        width: 30,
        height: 50,
    };

    println!("rectangle3: {:?} {}", rect, area(&rect));
}

impl Rectangle {
    // =========== methods ==============
    fn area(&self) -> u32 {
        self.width * self.height
    }

    fn can_hold(&self, other: &Rectangle) -> bool {
        self.width > other.width && self.height > other.height
    }

    fn enlarge(&mut self, by: u32) {
        // non-const method
        self.width += by;
        self.height += by;
    }

    fn saturate_to_square(self) -> Self {
        // self as value type
        let large = std::cmp::max(self.width, self.height);
        Rectangle {
            width: large,
            height: large,
        }
    }

    // =========== associate functions ==============
    fn square(size: u32) -> Rectangle {
        Rectangle {
            width: size,
            height: size,
        }
    }
}

fn rectangle_method() {
    let mut rect = Rectangle {
        width: 30,
        height: 50,
    };
    let rect2 = Rectangle {
        width: 20,
        height: 40,
    };

    let s = Rectangle::square(30);

    println!("rectangle_method: rect {:?} {}", rect, rect.area());
    rect.enlarge(10);
    println!("rectangle_method: enlared rect {:?} {}", rect, rect.area());

    println!(
        "rectangle_method: {:?} can_holds {:?} -> {}",
        rect,
        rect2,
        rect.can_hold(&rect2)
    );
    println!("rectangle_method: square {:?}", s);

    let rect_square = rect.saturate_to_square();
    println!("rectangle_method: rect square {:?}", rect_square);
}

struct Builder {
    i: i32,
}

impl Builder {
    fn inc(&mut self) -> &mut Self {
        self.i += 1;
        self
    }

    fn binc(mut self) -> Self {
        self.i += 1;
        self
    }

    fn build(self) -> i32 {
        self.i
    }
}

fn test_Builder() {
    let mut builder = Builder { i: 0 };
    builder.inc().inc();
    let a = builder.build();

    let b = Builder { i: 0 }.binc().binc().build();

    // let mut ab = Builder{i:0};
    // ab.inc();
    // ab.inc();
    // let abi = ab.build();
    // ab.inc();
}
