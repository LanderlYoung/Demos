//
//  MethodSwizzling.h
//  LearnOC
//
//  Created by laderlyoung on 2022/8/30.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface MethodSwizzling : NSObject

- (void)hello;

@end

@interface MethodSwizzlingSub : MethodSwizzling

@end

NS_ASSUME_NONNULL_END
