pub fn test() {
    let a = String::from("hello");
    let b = "world";

    let longest = longest(&a, b);

    println!("longest is {longest}");
}

// 所有标注 'a 的变量声明周期都至少是 'a
// 因此对于参数而言，'a 的lifetime实际上是 min(a, b) 两个引用的
fn longest<'a>(a: &'a str, b: &'a str) -> &'a str {
    if a.len() > b.len() {
        a
    } else {
        b
    }
}

/*

fn test_lifetime() {
    let a = String::from("hello");
    let l;
    {
        let b = String::from("world");
    // error[E0597]: `b` does not live long enough
        l = longest(&a, &b);
    }
    println!("test_lifetime, longest is {l}");
}

*/

struct StructWithReference<'a> {
    name: &'a str, // name 的lifetime 要 >= 结构体本身
    age: u32,
}

impl<'a> StructWithReference<'a> {
    fn longest1(&'a self, other: &'a str) -> &'a str {
        if self.name.len() > other.len() {
            &self.name
        } else {
            other
        }
    }
}

fn test_struct_with_reference() {
    let mut s = StructWithReference {
        name: "hello",
        age: 18,
    };

    {
        s.age = 20;
        let ss = String::from("world");

        // `ss` does not live long enough
        // s.name = &ss;
    }

    println!("StructWithReference::name {}", s.name);
}

struct S1 {
    name: String,
}

impl S1 {
    fn longest1(&self, other: &str) -> &str {
        &self.name
    }

    // 按照rule-3，返回值的lifetime默认是self
    // 但是代码实现和这个lifetime声明违背了，因此编译不过
    /*
    fn longest1(&self, other: &str) -> &str {
        if self.name.len() > other.len() {
            &self.name
        } else {
             other // 编译不过
        }
    }
    */

    fn longest2<'a>(&'a self, other: &'a str) -> &'a str {
        if self.name.len() > other.len() {
            &self.name
        } else {
            other
        }
    }
}

fn lifetime_elision1(a: &str) -> &str {
    println!("{a}");
    "hello"
}
// fn lifetime_elision1<'a>(a: &'a str) -> &'a str

fn lifetime_elision2(a: &str) -> &'static str {
    println!("{a}");
    "hello"
}

fn test_LE() {
    {
        // case 1
        let r: &str;
        {
            let ss = String::from("hello");
            // error[E0597]: `ss` does not live long enough
            // r = lifetime_elision1(&&ss);
        }

        // println!("{r}");
    }

    {
        //case 2
        let r;

        {
            let ss = String::from("hello");
            r = lifetime_elision2(&&ss);
        }

        println!("{r}");
    }
}
