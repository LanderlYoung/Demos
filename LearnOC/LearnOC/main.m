//
//  main.m
//  Objective-C
//
//  Created by landerlyoung on 2021/7/26.
//

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import "ClassInspector.h"
#import "SimpleTest1.h"
#import "PropertyTest.h"
#import "KVOTest.h"
#import "MethodSwizzling.h"
#import "DeallocOrderTest.h"
#import "ClassProtocol.h"

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        // insert code here...
        NSLog(@"Hello, World!");
        NSLog(@"%@", [[NSBundle mainBundle] bundleURL].path);
        
        id myClass = [SimpleTest1 runTest];
        [DeallocOrderTest test];
        
        NSLog(@"================");
        [ClassProtocol runTest];

        NSLog(@"================");
        NSString *message = [myClass inspect];
//        NSLog(@"\n%@", message);
        
        PropertyTest* pt = [PropertyTest runTest];
//        NSLog(@"\n%@", [pt inspect]);
        KVOTest *kvo = [KVOTest runTest];
        
//
        NSLog(@"================");
        NSLog(@"\n%@", [NSObject inspectClass:[NSArray class]]);
//
        NSLog(@"================");
        NSLog(@"\n%@", [NSObject inspectClass:[NSView class]]);
        
        NSString *s = @"The weather on \U0001F30D is \U0001F31E today.e\u0301";
            // The weather on 🌍 is 🌞 today.
        NSRange fullRange = NSMakeRange(0, [s length]);
        __block NSInteger count = 0;
        [s enumerateSubstringsInRange:fullRange
                              options:NSStringEnumerationByComposedCharacterSequences
                           usingBlock:^(NSString *substring, NSRange substringRange,
                                        NSRange enclosingRange, BOOL *stop)
        {
            count++;
            NSLog(@"%@ %@ %@", substring, NSStringFromRange(substringRange), @(count));
        }];
        NSLog(@"%@ %@", s, @(count));
        
        /////
        
        [[[MethodSwizzlingSub alloc] init] hello];
    }
    return 0;
}
