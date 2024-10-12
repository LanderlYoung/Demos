//
//  DeallocOrder.h
//  LearnOC
//
//  Created by laderlyoung on 2022/10/10.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface DeallocOrder : NSObject

@property(nonatomic, copy) NSString *name;

@property(nonatomic, strong) id obj;

+ (void)test;

@end

NS_ASSUME_NONNULL_END
