//
//  KotlinObjcBridge.m
//  iosApp
//
//  Created by landerlyoung on 2023/1/27.
//  Copyright © 2023 orgName. All rights reserved.
//

#import "KMMBridge.h"
#import <Kt/Kt.h>

@implementation KMMBridge

+ (void)getLastSuccessfulLaunchWithCallback:(void(^)(NSString*, NSError*)) callback {
    if (!callback) {
        return;
    }
    
    [[[KtGreeting alloc] init] greetingWithCompletionHandler:^(NSString * str, NSError * err) {
        dispatch_async(dispatch_get_main_queue(), ^() {
            callback(str,err);
        });
    }];
}

@end
