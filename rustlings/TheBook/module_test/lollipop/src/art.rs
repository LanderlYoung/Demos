pub mod gc;

pub struct ArtVm {
    heap: u32,
}

impl ArtVm {
    pub fn new() -> Self {
        Self { heap: 0 }
    }

    pub fn create_object(&mut self, size: u32) -> Option<u32> {
        if size > 128 {
            None
        } else {
            self.heap += size;
            Some(size)
        }
    }
}
