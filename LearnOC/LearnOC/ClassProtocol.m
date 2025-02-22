//
//  ClassProtocol.m
//  LearnOC
//
//  Created by landerlyoung on 2024/12/11.
//

#import "ClassProtocol.h"


@protocol ProtocolWithClassMethod

- (NSString *)instenceMethod;
+ (NSString *)classMethod;

@end

@implementation ClassProtocol

+ (void)runTest {
    [self useClassComformToProtocol: self];
}

+ (void)useClassComformToProtocol:(Class<ProtocolWithClassMethod>) class {
    [class classMethod];
}

@end
