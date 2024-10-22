
fn main() {
    println!("If");

    let condition = true;
    // let val = if condition 5 else 6; // 强制要求 {}
    let val = if condition { 5 } else { 6 }; // 强制要求 {}

    if condition {
        println!("cond");
    } else if val * val  == 4  {
        println!("cond");
    }

    println!("Loop: loop");

    let mut index = 0;
    loop { // 等价于 while true
        if index > 3 {
            break;
        }
        println!("loop {index}");
        index += 1;
    }

    // loop with return value
    let val = loop {
        if index > 5 {
            break index * index;
        }

        index += 1;
    };

    // nested loop with label
    let x = 'outer_loop: loop {
        // inner loop
        loop {
            break 'outer_loop 10;
        }
    };

    println!("{val} {x}");

    println!("Loop: while");

    while index < 6 {
        println!("while loop: {index}");
        index += 1;
    }

    println!("Loop: for-in");
    let a = [10, 20, 30, 40];
    for element in a {
        println!("for-in: {element}");
    }

    // range and reverse
    for number in (1..4).rev() {
        println!("count down: {number}!")
    }

    println!("fibo(5) = {}", fibo(5));
}

fn fibo(index: i32) -> i64 {
    if index == 0 || index == 1 {
        return 1;
    }
    fibo(index - 1) + fibo(index - 2)
}
