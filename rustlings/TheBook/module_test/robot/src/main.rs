fn main() {
    test_kitkat();
    test_lollipop();
}

#[allow(unused_imports)]
fn test_kitkat() {
    use kitkat::{self, add, hello_holo, HoloTheme, Kitkat};
    let kk = Kitkat::new("Galaxy Nexus");
    println!("kitkat greed: {}", kk.greet());
    println!("kitkat version {}", kk.version());
    // kk.set_version(0); // not accessible
    println!("kitkat::snake::sweet: {}", kitkat::snake::sweet("crab"));

    let _theme = kitkat::HoloTheme::Light { brightness: 128.0 };

    println!("Hello, world! {} ", kitkat::add(1, 2),);
}

fn test_lollipop() {
    println!("{}", lollipop::lollipop(10));

    let mut vm = lollipop::ArtVm::new();
    let obj1 = vm.create_object(64);
    let obj2 = vm.create_object(256);

    println!("vm created obj {obj1:?} {obj2:?}");

    let collected = lollipop::gc::perform_gc(&mut vm);
    println!("gc collect {}bytes", collected);

    println!("lollipop material: {}", lollipop::material_design::material_you("Nexus 6"));
}
