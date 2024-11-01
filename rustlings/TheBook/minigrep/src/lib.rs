// this is the content of `mod minigrip`
use std::io::{self};

pub struct Config<'a> {
    pattern: &'a str,
    file_name: &'a str,
}

impl<'a> Config<'a> {
    pub fn new(args: &'a Vec<String>) -> Result<Config<'a>, String> {
        if args.len() < 3 {
            Err(format!(
                "no enough arguments: {} <pattern> <file_name>",
                args[0]
            ))
        } else {
            Ok(Config {
                pattern: &args[1],
                file_name: &args[2],
            })
        }
    }
}

pub struct MiniGrep {
    file_content: String,
    pattern: String,
    index: usize,
}

impl MiniGrep {
    pub fn new(config: &Config) -> io::Result<MiniGrep> {
        Ok(Self {
            file_content: std::fs::read_to_string(&config.file_name)?,
            pattern: config.pattern.to_string(),
            index: 0,
        })
    }

    pub fn next(&mut self) -> Option<&str> {
        while self.index < self.file_content.len() {
            let start = self.index;
            let mut line_end = start + 1;
            while line_end < self.file_content.len()
                && self.file_content.as_bytes()[line_end] != b'\n'
            {
                line_end += 1
            }
            self.index = line_end;

            let line = &self.file_content[start..line_end];
            if line.contains(&self.pattern) {
                return Some(&line);
            }
        }

        None
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn option_parse() {
        let args = vec![String::from("cmd"), String::from("pattern")];
        let r = Config::new(&args);
        assert!(r.is_err());

        let args = vec![String::from("cmd"), String::from("pattern"), String::from("file.txt")];
        let r = Config::new(&args);
        assert!(r.is_ok());
        let config = r.unwrap();
        assert_eq!(config.file_name, "file.txt");
        assert_eq!(config.pattern, "pattern");
    }
}
