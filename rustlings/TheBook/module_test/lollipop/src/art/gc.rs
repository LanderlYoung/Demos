use super::ArtVm;

pub fn perform_gc(vm: &mut ArtVm) -> u32 {
    let ret = vm.heap;
    vm.heap = 0;
    ret
}
