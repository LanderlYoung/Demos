//
//  KVOTest.m
//  KVOTest
//
//  Created by landerlyoung on 2021/9/7.
//
// KVOed property have different calling stack.
//

#import "KVOTest.h"

static void* contextIvarInt = &contextIvarInt;
static void* contextIvarString = &contextIvarString;

static void* contextPropInt = &contextPropInt;
static void* contextPropString = &contextPropString;
static void* contextPropNoBacking = &contextPropNoBacking;
static void* contextSubTest_propString = &contextSubTest_propString;

@interface KVObserver : NSObject

- (void)observeValueForKeyPath:(nullable NSString *)keyPath ofObject:(nullable id)object change:(nullable NSDictionary<NSKeyValueChangeKey, id> *)change context:(nullable void *)context;

@end

@implementation KVObserver

- (void)observeValueForKeyPath:(nullable NSString *)keyPath
                      ofObject:(nullable id)object
                        change:(nullable NSDictionary<NSKeyValueChangeKey, id> *)change
                       context:(nullable void *)context {
    
    NSLog(@"changed for key %@ change%@", keyPath, change);
}
 
@end

@implementation KVOTest

//
// KVOed property
// * thread #1, queue = 'com.apple.main-thread', stop reason = breakpoint 2.1
//* frame #0: 0x0000000100002894 LearnOC`-[KVOTest setPropInt:](self=0x00000001006066b0, _cmd="setPropInt:", propInt=42) at KVOTest.m:47:6
//  frame #1: 0x00007fff2118a932 Foundation`-[NSObject(NSKeyValueObservingPrivate) _changeValueForKeys:count:maybeOldValuesDict:maybeNewValuesDict:usingBlock:] + 650
//  frame #2: 0x00007fff211b5ad6 Foundation`-[NSObject(NSKeyValueObservingPrivate) _changeValueForKey:key:key:usingBlock:] + 68
//  frame #3: 0x00007fff2126b918 Foundation`_NSSetIntValueAndNotify + 266
//  frame #4: 0x0000000100002d29 LearnOC`+[KVOTest runTest](self=KVOTest, _cmd="runTest") at KVOTest.m:82:10
//  frame #5: 0x00000001000022ec LearnOC`main(argc=1, argv=0x00007ffeefbff438) at main.m:29:24
//  frame #6: 0x00007fff20357f3d libdyld.dylib`start + 1
//  frame #7: 0x00007fff20357f3d libdyld.dylib`start + 1
-(void) setPropInt:(int)propInt {
    // porpNoBacking is computed by propInt, we must notify them manually
    [self willChangeValueForKey:@"porpNoBacking"];
    
    // calling stack trace
    // "setPropInt: call to KVO prop setter
    
    [self didChangeValueForKey:@"porpNoBacking"];
    
    // other ivar can also be notify like such
    // [self willChangeValueForKey:@"ivarInt"];
    // [self didChangeValueForKey:@"ivarInt"];
}

// not KVOed property
//* thread #1, queue = 'com.apple.main-thread', stop reason = breakpoint 3.1
//  * frame #0: 0x00000001000028bf LearnOC`-[KVOTest setPropInt2:](self=0x000000010783c760, _cmd="setPropInt2:", propInt2=43) at KVOTest.m:64:17
//    frame #1: 0x0000000100002d5b LearnOC`+[KVOTest runTest](self=KVOTest, _cmd="runTest") at KVOTest.m:92:10
//    frame #2: 0x00000001000022ec LearnOC`main(argc=1, argv=0x00007ffeefbff438) at main.m:29:24
//    frame #3: 0x00007fff20357f3d libdyld.dylib`start + 1
//    frame #4: 0x00007fff20357f3d libdyld.dylib`start + 1
- (void)setPropInt2:(int)propInt2 {
    // calling stack trace
    _propInt2 = propInt2;
}

- (NSNumber*) porpNoBacking {
    return @(self.propInt * 42);
}

- (void)dealloc {
    NSLog(@"KVOTest dealloc");
}

- (void)addObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath options:(NSKeyValueObservingOptions)options context:(void *)context {
    [super addObserver:observer forKeyPath:keyPath options:options context:context];
    
    NSLog(@"KVOTest self:%@ addObserver:%@ forKeyPath:%@", self, observer, keyPath);
}

- (void)removeObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath context:(void *)context {
    [super removeObserver:observer forKeyPath:keyPath context:context];
    NSLog(@"KVOTest self:%@ removeObserver:%@ forKeyPath:%@ context:%p", self, observer, keyPath, context);
}

- (void)removeObserver:(NSObject *)observer forKeyPath:(NSString *)keyPath {
    [super removeObserver:observer forKeyPath:keyPath];
    NSLog(@"KVOTest self:%@ removeObserver:%@ forKeyPath:%@", self, observer, keyPath);
}

+ (instancetype)runTest {
    KVOTest* test = [[KVOTest alloc] init];
    KVObserver*ob = [[KVObserver alloc]init];
    // keyPath, contexg
    NSDictionary<NSString*, NSValue*>* props = @{
        @"propInt": [NSValue valueWithPointer: contextPropInt],
        @"propString": [NSValue valueWithPointer: contextPropString],
        @"porpNoBacking": [NSValue valueWithPointer: contextPropNoBacking],
        @"ivarInt":[NSValue valueWithPointer: contextIvarInt],
        @"ivarString": [NSValue valueWithPointer: contextIvarString],
        // subTest.propString
        @"subTest.propString": [NSValue valueWithPointer: contextSubTest_propString],
    };
    
    NSLog(@"KVOTest begin addObserver");
    for (NSString* prop in props) {
        [test addObserver:ob
               forKeyPath:prop
                  options: (NSKeyValueObservingOptionNew | NSKeyValueObservingOptionOld)
                  context: [props[prop] pointerValue]
        ];
    }
    NSLog(@"KVOTest end addObserver");

    NSLog(@"KVOTest begin changes");
    test.propInt = 42;
    test.propInt = 43;
    test.propInt2 = 43;
    test.propString = @"hello";
    test.propString = @"work";
    test->ivarInt = 10;
    test->ivarInt = 11;
    test->ivarString = @"a";
    test->ivarString = @"b";
    
    NSLog(@"KVOTest assign subTest");
    test.subTest = [[KVOTest alloc] init];
    test.subTest.propString = @"subTest_string";

    // test.subTest = nil;
    NSLog(@"KVOTest end changes");
    
    for (NSString* prop in props) {
        [test removeObserver:ob forKeyPath: prop];
    }
    
    test.propInt = 42;
    test.propInt = 43;
    test.propInt2 = 43;
    test.propString = @"hello";
    test.propString = @"work";
    test->ivarInt = 10;
    test->ivarInt = 11;
    test->ivarString = @"a";
    test->ivarString = @"b";
    
    return test;
}

@end
