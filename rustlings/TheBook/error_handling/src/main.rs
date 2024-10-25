use std::io::{self, Error, ErrorKind, Read, Write};

use std::fs::{self, File};
use std::num::ParseIntError;
use std::ops::ControlFlow;

fn main() -> Result<(), ParseIntError>{
    let mut file_name = String::new();
    print!("Please input file name: ");
    let _ = io::stdout().flush();

    io::stdin()
        .read_line(&mut file_name)
        .expect("Failed to intput file name");

    let file_name = file_name.trim();

    // open_file3(file_name);

    match read_file2(file_name) {
        Ok(content) => println!("file {file_name} content:\n{content}"),
        Err(e) => print!("failed to open {file_name} {e:?}"),
    }

    let _ = try_return_or_break("non-existed-file");

    println!("\n===============");
    // return error in main
    "1x2".parse::<i32>()?;
    Ok(())
}

#[allow(unused)]
fn open_file(file_name: &str) -> File {
    let file = File::open(&file_name);
    let file = match file {
        Ok(file) => {
            println!("open file {} success", file_name);
            file
        }
        Err(e) => match e.kind() {
            ErrorKind::NotFound => match File::create(&file_name) {
                Ok(f) => {
                    println!("created file {file_name}");
                    f
                }
                Err(e) => panic!("failed to create file {} {:?}", file_name, e),
            },
            _ => panic!("failed to open file {} {:?}", file_name, e),
        },
    };
    file
}

#[allow(unused)]
fn open_file2(file_name: &str) -> File {
    File::open(&file_name).unwrap_or_else(|error| {
        if error.kind() == ErrorKind::NotFound {
            File::create(file_name)
                .unwrap_or_else(|error| panic!("failed to create file {file_name} {error:?}"))
        } else {
            panic!("failed to open file {file_name} {error:?}")
        }
    })
}

#[allow(unused)]
fn open_file3(file_name: &str) -> File {
    File::open(&file_name).unwrap_or_else(|error| {
        if error.kind() == ErrorKind::NotFound {
            File::create(file_name).expect("failed to create file")
        } else {
            panic!("failed to open file {file_name} {error:?}")
        }
    })
}

fn read_file1(file_name: &str) -> Result<String, Error> {
    let mut file = match File::open(file_name) {
        Ok(f) => f,
        Err(e) => return Err(e),
    };

    let mut content = String::new();
    return match file.read_to_string(&mut content) {
        Ok(_) => Ok(content),
        Err(e) => Err(e),
    };
}

fn read_file2(file_name: &str) -> Result<String, Error> {
    // ? is the ops::Try trait
    let mut file = File::open(file_name)?;
    let mut content = String::new();
    file.read_to_string(&mut content)?;
    Ok(content)
}

fn read_file3(file_name: &str) -> Result<String, Error> {
    // ? is the ops::Try trait
    let mut content = String::new();
    File::open(file_name)?.read_to_string(&mut content)?;
    Ok(content)
}

fn read_file4(file_name: &str) -> Result<String, Error> {
    fs::read_to_string(file_name)
}

fn try_return_or_break(file_name: &str) -> io::Result<String> {
    loop {
        let mut file = File::open(file_name)?;

        println!("try_return_or_break loop body");
        break;
    }

    println!("try_return_or_break function body");
    Ok("yes".to_string())
}
