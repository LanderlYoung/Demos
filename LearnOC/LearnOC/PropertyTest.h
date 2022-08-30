//
//  PropertyTest.h
//  PropertyTest
//
//  Created by landerlyoung on 2021/9/7.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface PropertyTest : NSObject {
    int age;
    NSString* name;
    // for property width,
    // if we omit this variable declration, compile will generate for us
    NSNumber* width;
}

///
///-(void)setWidth:(void *)arg2 {
///    objc_setProperty_atomic_copy(self, _cmd, arg2, 0x18);
///    return;
///}
///  objc_setProperty_atomic_copy
/// reallySetProperty
/// https://opensource.apple.com/source/objc4/objc4-551.1/runtime/Accessors.subproj/objc-accessors.mm.auto.html
/// https://juejin.cn/post/6918705861967757325
///
@property(copy) NSNumber* width;

///-(void)setHeight:(void *)arg2 {
///    objc_storeStrong(self + 0x20, arg2);
///    return;
///}
@property(nonatomic, retain) NSNumber* height;

@property(nonatomic, readonly) NSNumber* area;

///-(void)setHeight2:(void *)arg2 {
///    objc_setProperty_nonatomic_copy(self, _cmd, arg2, 0x28);
///    return;
///}
///  objc_setProperty_nonatomic_copy
///
@property(nonatomic, copy) NSNumber* height2;

@property(nonatomic, retain, readonly) NSString* title;

+(instancetype) runTest;
@end

NS_ASSUME_NONNULL_END
