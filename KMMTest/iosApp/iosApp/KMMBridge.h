//
//  KotlinObjcBridge.h
//  iosApp
//
//  Created by landerlyoung on 2023/1/27.
//  Copyright © 2023 orgName. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface KMMBridge : NSObject

+ (void)getLastSuccessfulLaunchWithCallback:(void(^)(NSString * _Nullable, NSError * _Nullable)) callback;

+ (void)mmBenchmark;

@end

NS_ASSUME_NONNULL_END
