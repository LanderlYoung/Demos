//
//  KVOTest.h
//  KVOTest
//
//  Created by landerlyoung on 2021/9/7.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/// https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/KeyValueObserving/Articles/KVOImplementation.html#//apple_ref/doc/uid/20002307-BAJEAIEE
@interface KVOTest : NSObject {
    int ivarInt;
    NSString* ivarString;
}

@property(nonatomic) int propInt;
@property(nonatomic) int propInt2;
@property(nonatomic) NSString *propString;
@property(nonatomic, readonly) NSNumber *porpNoBacking;
@property(nonatomic, strong) KVOTest *subTest;

+(instancetype) runTest;

@end

NS_ASSUME_NONNULL_END

