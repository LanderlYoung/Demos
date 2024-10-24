// test1: crate with modules and sub modules in one file

// this file is the content of mod kitkat,  some like:
// pub mod { /* content of this file */ }

// reexport private child
pub use holo::hello_holo;
pub use holo::HoloTheme;

#[derive(Debug)]
pub struct Kitkat {
    name: String, // private
    version: u32, // private
}

impl Kitkat {
    pub fn new(name: &str) -> Self {
        Self {
            name: name.to_string(),
            version: 19,
        }
    }

    pub fn version(&self) -> u32 {
        self.version
    }

    pub fn greet(&self) -> String {
        // cannot access sub module private method
        // function `submodule_fn` is private
        // let num = Holo::submodule_fn();

        format!(
            "kitkat {} {}",
            hello_holo(&self.name, &self),
            holo::submodule_fn2()
        )
        // or ; Holo::hello_holo(&self.name)
        // or absolute path; crate::Holo::hello_holo(&self.name)
    }

    #[allow(unused)]
    fn set_version(&mut self, version: u32) {
        // private
        self.version = version
    }
}

pub fn add(left: u64, right: u64) -> String {
    format!("kitkat_add={}", left + right)
}

mod holo {
    use crate::Kitkat;

    pub enum HoloTheme {
        Dark,
        Light { brightness: f32 },
    }

    pub fn hello_holo(name: &str, kk: &Kitkat) -> String {
        // submodule can access super module private's
        format!("hello holo {name}, API-{}", kk.version)
    }

    fn submodule_fn() -> u32 {
        42
    }

    // mannually annotate it's visible to the enitire crate
    pub(crate) fn submodule_fn2() -> u32 {
        submodule_fn()
    }
}

pub mod snake {
    pub fn sweet(what: &str) -> String {
        format!("sweet kitkat {what}")
    }
}
