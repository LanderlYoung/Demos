//
//  ClassInspector.h
//  LearnOC
//
//  Created by landerlyoung on 2021/8/16.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSObject(ClassInspector)
- (NSString*) inspect;
+ (NSString*) inspectClass:(Class) class;
@end

NS_ASSUME_NONNULL_END
