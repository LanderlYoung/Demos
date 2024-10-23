#[derive(Debug)]
enum Command {
    Start,
    Rotate { degree: u32 }, // variant like struct
    Translate(u32, u32), // variant like tuple
    Color(Color), // variant like tuple, x.0 is type of another struct
    End,
}

#[derive(Debug)]
struct Color {
    red: u8,
    green: u8,
    blue: u8,
}

impl Command {
    fn call(&self) {
        print!("execute Command {self:?}: ");
    }
}

impl Color {
    fn blend(&self) {
        println!("Blend color")
    }
}

// error[E0573]: expected type, found variant `Command::Translate`
//   --> src/main.rs:21:6
//    |
// 21 | impl Command::Translate { }
//    |      ^^^^^^^^^^^^^^^^^^
//    |      |
//    |      not a type
//    |      help: try using the variant's enum: `crate::Command`
// impl Command::Translate { }

fn execute_command(cmd: &Command) {
    cmd.call();
    match cmd {
        Command::Rotate { degree } => println!("Rorate({})", degree),
        Command::Translate(x, y) => println!("Translate({x}, {y})"),
        // Command::Color(Color{red, green, blue}) =>
        //        println!("color({red}, {green}, {blue}"),
        Command::Color(color) => {
            let Color { red, green, blue } = color;
            println!("Color({red}, {green}, {blue})");
            color.blend();
        }
        // match exepression must be exhausted
        // this is the else branch
        c => println!("{:?}", c),
        // or use `_` to match else but don't get the value
        // _ => {}
        // Command::Start => println!("Start"),
        // Command::End => println!("End"),
    }
}

fn test_command() {
    execute_command(&Command::Start);
    execute_command(&Command::Rotate { degree: 180 });
    execute_command(&Command::Translate(10, 20));
    execute_command(&Command::Color(Color {
        red: 10,
        green: 20,
        blue: 30,
    }));
    execute_command(&Command::End);
}

fn roll_dice(input: u32) -> Option<u32> {
    if input % 2 == 0 {
        None
    } else {
        Some(input)
    }
}

fn inc(input: &Option<u32>) -> Option<u32> {
    match input {
        Some(x) => Some(x + 1),
        None => None,
    }
}

fn inc1(input: &Option<u32>) -> Option<u32> {
    if let Some(x) = input {
        Some(x + 1)
    } else {
        None
    }
}

fn inc2(input: &Option<u32>) -> Option<u32> {
    input.map(|x| x + 1)
}

fn main() {
    test_command();

    let num = roll_dice(1);
    println!("{num:?} {:?} {:?} {:?}", inc(&num), inc1(&num), inc2(&num));

    let num = roll_dice(2);
    println!("{num:?} {:?} {:?} {:?}", inc(&num), inc1(&num), inc2(&num));
}
