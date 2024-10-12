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
@end

@implementation GCTest

- (void)dealloc {
    NSLog(@"Dealloc %@", self);
}

@end

@implementation KMMBridge

+ (void)getLastSuccessfulLaunchWithCallback:(void(^)(NSString*, NSError*)) callback {
    if (!callback) {
        return;
    }
    
    KtGreeting* greeting = [[KtGreeting alloc] init];
    greeting.any = [[GCTest alloc] init];
    
    [greeting greetingWithCompletionHandler:^(NSString * str, NSError * err) {
        dispatch_async(dispatch_get_main_queue(), ^() {
            callback(str,err);
        });
    }];
}

@end
