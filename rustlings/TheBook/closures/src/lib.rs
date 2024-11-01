#[cfg(test)]
mod tests {

    #[test]
    fn closure_capture_types() {
        let name = String::from("hello");
        let mut age = 18;
        let mut v = Vec::<String>::new();
        let treasure = String::from("gold");

        let closure = || {
            println!("name: {name:?}"); // borrowed as immutable reference
                                        // just like: let borrowed_name: &String = &name;

            age += 1; // as mutable reference
                      // just like: let borrowed_age: &mut i32 = &mut age;

            // treasure is move borrow! i.e. move value into closure
            // just like: let borrowed_treasure: String = treasure;
            v.push(treasure); // note in this line, treasure is also moved out of closure body
                              // so closure doesn't own treasure anymore, thus closure is also moved

            // the above line, v as mutable reference
        };

        closure();
        println!("name:{name:?} age:{age:?} v:{v:?}")
    }

    #[test]
    fn closure_traits() {
        let name = String::from("hello");
        let mut age = 18;
        let mut v = Vec::<String>::new();

        let fn1 = || {}; // no capture
        let fn2 = || println!("{name}"); // name as `&``

        let fnmut1 = || v.push(String::from("Bar")); // v as `&mut`
        let fnmut2 = || {
            println!("{name}"); // as `&`
            age += 1; // as `&mut`
        };

        let fnonce1 = || {
            name // take ownership, and move out of closure body (as return value)
        };

        let name2 = String::from("hello");
        let fnonce2 = || {
            v.push(name2); // take ownership of name2, and move out of closure body (into v)
        };

        let name3 = String::from("hello");
        let fnonce3 = || {
            let mut my_v = Vec::<String>::new();
            my_v.push(name3);
            // 即使my_v在closure内部，但是当call之后，my_v作为临时变量被丢弃了，此时仍然是out of body
        };

        let name4 = String::from("hello");
        let x = move || {
            println!("{name4}");
        };

    }

    #[test]
    fn closure_immutable_borrow() {
        let v = vec![1, 2, 3, 4, 5];

        println!("before define closure {v:?}");
        let borrow_immutably = || println!("in closure: {v:?}");
        println!("after define closure {v:?}");
        borrow_immutably();
        println!("after call closure {v:?}");
    }

    #[test]
    fn closure_mutable_borrow() {
        let mut v = vec![1, 2, 3, 4, 5];

        println!("before define closure {v:?}");
        let mut borrow_mutably = || v.push(0);

        // error[E0502]: cannot borrow `v` as immutable because it is also borrowed as mutable
        // println!("vector is {v:?}");

        borrow_mutably();
        println!("vector is {v:?}");

        /*
        borrow_mutably MUST be declated as `mut`

        error[E0596]: cannot borrow `borrow_mutably` as mutable, as it is not declared as mutable
        13 |         borrow_mutably();
           |         ^^^^^^^^^^^^^^ cannot borrow as mutable
         */
    }

    #[test]
    fn closure_move_borrow() {
        let x = String::from("42");

        println!("before closure define {x:?}");
        let closure = move || println!("inside closure {x:?}");

        // error[E0382]: borrow of moved value: `x`
        // println!("after closure define {x:?}");
        closure();
        closure();

        // error[E0382]: borrow of moved value: `x`
        // println!("after closure call {x:?}");

        /*
          x 已经move进 closure了，后续不能再使用。
          如果 x 实现了copy Trait，则会copy进去，后续允许继续使用。
        */
    }

    #[test]
    fn closure_move_capture_out() {
        let x = String::from("42");

        println!("before closure define {x:?}");
        // 即使没有move关键字，closure对x的使用也是move形式的，因此x就是move捕获
        let closure = || {
            println!("inside closure {x:?}");
            x // closure moves its capture out of it's body
        };

        closure();

        // 不允许再次调用closure，因为他捕获的变量已经move出去了，closure自己也处于已经move的状态
        // error[E0382]: use of moved value: `closure`
        // closure();
    }
}

