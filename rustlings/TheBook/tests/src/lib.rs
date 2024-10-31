pub fn add(left: u64, right: u64) -> u64 {
    left + right
}

#[derive(Debug)]
struct Rect {
    width: u32,
    height: u32,
}

impl Rect {
    fn area(&self) -> u32 {
        self.width * self.height
    }

    fn pan(&self) {
        panic!("panic {self:?}")
    }
}

impl From<(u32, u32)> for Rect {
    fn from(value: (u32, u32)) -> Self {
        Self {
            width: value.0,
            height: value.1,
        }
    }
}

#[cfg(test)] // ONLY compiled on test build
mod tests {
    use super::*;

    #[test] // test function
    fn asserts() {
        assert!(true);
        assert_eq!(1, 1);
        assert_ne!(1, 2);
    }

    #[test]
    #[ignore] // ignored by default, run with `cargo test -- --ignored`
    fn area() {
        let a = Rect {
            width: 10,
            height: 12,
        };
        assert_eq!(a.area(), 120);
    }

    #[test]
    fn from() {
        let a = Rect::from((10, 12));
        assert_eq!(a.width, 10);
        assert_eq!(a.height, 12);

        let a: Rect = (10, 12).into();
        assert_eq!(a.width, 10);
        assert_eq!(a.height, 12);
    }

    #[test]
    #[should_panic]
    fn pan() {
        let a = Rect::from((10, 12));
        a.pan();
    }
}
