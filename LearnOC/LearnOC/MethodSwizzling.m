//
//  MethodSwizzling.m
//  LearnOC
//
//  Created by laderlyoung on 2022/8/30.
//

#import "MethodSwizzling.h"
#import <objc/runtime.h>

@implementation MethodSwizzling

- (void)hello {
    NSLog(@"%@ hello %@", NSStringFromClass(self.class), NSStringFromSelector(_cmd));
}

@end

@implementation MethodSwizzlingSub

@end

@interface MethodSwizzling (Ext)

- (void)wrapHello;

@end

@implementation MethodSwizzling (Ext)

#define SWIZZLING_TYPE 0

#if SWIZZLING_TYPE == 0
+ (void)initialize {
    if (self.class == MethodSwizzling.class) {
        static dispatch_once_t once;
        dispatch_once(&once, ^{
            Class clazz = MethodSwizzling.class;
            Method helloMethod = class_getInstanceMethod(clazz, @selector(hello));
            Method wrapHelloMethod = class_getInstanceMethod(clazz, @selector(wrapHello));
            IMP realHelloImp = method_getImplementation(helloMethod);
           
            method_exchangeImplementations(helloMethod, wrapHelloMethod);
            
            IMP wrapHelloImp2 = method_getImplementation(wrapHelloMethod);
            Method wrapHelloMethod2 = class_getInstanceMethod(clazz, @selector(wrapHello));
            IMP wrapHelloImp3 = method_getImplementation(wrapHelloMethod2);
            
            NSAssert(wrapHelloMethod == wrapHelloMethod2, @"WTF1");
            NSAssert(wrapHelloImp2 == wrapHelloImp3, @"WTF2");
            NSAssert(wrapHelloImp2 == realHelloImp, @"WTF3");
        });
    }
}


- (void)wrapHello {
    NSLog(@"before WrapHello %@", NSStringFromSelector(_cmd));
    
    // calls to the real hello method
    [self wrapHello];
    
    NSLog(@"after WrapHello");
}
#endif

#if SWIZZLING_TYPE == 1

static IMP realHelloImp;

+ (void)initialize {
    if (self.class == MethodSwizzling.class) {
        static dispatch_once_t once;
        dispatch_once(&once, ^() {
            Class clazz = MethodSwizzling.class;
            Method helloMethod = class_getInstanceMethod(clazz, @selector(hello));
            realHelloImp = method_getImplementation(helloMethod);
            
            Method wrapHelloMethod = class_getInstanceMethod(clazz, @selector(wrapHello));
            IMP wrapHelloImp = method_getImplementation(wrapHelloMethod);
            
            method_setImplementation(helloMethod, wrapHelloImp);
        });
    }
}

- (void)wrapHello {
    NSLog(@"before WrapHello %@", NSStringFromSelector(_cmd));
    
    typedef void(*HelloFunctionType)(id, SEL);
    
    // calls to the real hello method
    ((HelloFunctionType)realHelloImp)(self, _cmd);
    
    NSLog(@"after WrapHello");
}

#endif

@end
