use tests::add;

#[test]
fn test1() {
    assert_eq!(add(1, 2), 3);
    // tests下的每一个文件都是单独的crate，和src下分开的
    // assert_eq!(crate::add(1, 2), 3);
    //                   ^^^ not found in the crate root
}

#[test]
fn test2() {}
