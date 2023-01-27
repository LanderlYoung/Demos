//
//  KotlinObjcBridge.m
//  iosApp
//
//  Created by landerlyoung on 2023/1/27.
//  Copyright © 2023 orgName. All rights reserved.
//

#import "KMMBridge.h"
#import <Kt/Kt.h>

@interface GCTest: NSObject
@property (nonatomic) id any;
@end

@implementation GCTest

- (void)dealloc {
//    NSLog(@"Dealloc %@", self);
}

@end

@implementation KMMBridge

+ (void)getLastSuccessfulLaunchWithCallback:(void(^)(NSString*, NSError*)) callback {
    if (!callback) {
        return;
    }

    KtGreeting* greeting = [[KtGreeting alloc] init];
    [greeting greetingWithCompletionHandler:^(NSString * str, NSError * err) {
        dispatch_async(dispatch_get_main_queue(), ^() {
            callback(str,err);
        });
    }];
}

static const NSInteger kBenchmarkCount = 1000 * 1000;

// returns ms
+ (NSTimeInterval)benchmarkRunBlock:(void (^)(void))block {
    NSTimeInterval start = [NSDate.date timeIntervalSince1970];
    for (NSInteger i = 0; i < kBenchmarkCount; i++) {
        block();
    }
    return ([NSDate.date timeIntervalSince1970] - start) * 1000;
}

+ (void)mmBenchmark {
    NSTimeInterval ktNew = [KtGcTest.companion benchmarkCount: kBenchmarkCount];
    NSTimeInterval oc2ktNew = [self benchmarkRunBlock:^{
        [[KtGcTest alloc] init];
    }];
    NSTimeInterval ocNew = [self benchmarkRunBlock:^{
        [[GCTest alloc] init];
    }];
    NSTimeInterval allNew = [self benchmarkRunBlock:^{
        [[KtGcTest alloc] init];
        [[GCTest alloc] init];
    }];
    NSTimeInterval allNewAndFinalize = [self benchmarkRunBlock:^{
        [[KtGcTest alloc] init].any = [[GCTest alloc] init];
    }];

    NSLog(@"\nEnd Create & GC test \n"
          @"ktNew:%.2fms\n"
          @"oc2ktNew:%.2fms\n"
          @"ocNew:%.2fms\n"
          @"allNew:%.2fms\n"
          @"allNewAndFinalize:%.2fms",
          ktNew, oc2ktNew, ocNew, allNew, allNewAndFinalize);
}
@end
