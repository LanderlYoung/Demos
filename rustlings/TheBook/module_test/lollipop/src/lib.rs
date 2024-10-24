// sub modules
mod art;
pub use art::ArtVm; // re export
pub use art::gc; // re export grand-child module
pub mod material_design;

pub fn lollipop(count: u32) -> String {
    let mut s = format!("give {count} lollipop");
    if count > 1 {
        s.push_str("s");
    }
    s
}
