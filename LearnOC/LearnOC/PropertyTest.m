//
//  PropertyTest.m
//  PropertyTest
//
//  Created by landerlyoung on 2021/9/7.
//

#import "PropertyTest.h"

@implementation PropertyTest

@synthesize width;


- (instancetype)init {
    self = [super init];
    _title = @"hello";
    return self;
}

-(NSNumber*) area {
    return @(self.width.doubleValue * self.height.doubleValue);
}

+(instancetype) runTest {
    PropertyTest* this = [[PropertyTest alloc] init];
    
    // readonly
    // this.title  = "world";
    this.width = @(10);
    this.height = this.width;
    this.height = @20;
    assert(this.area.intValue == 200);
    
    this->age = 18;
    this->name = @"ABC";
    
    return this;
}

@end
