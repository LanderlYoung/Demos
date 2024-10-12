//
//  DeallocOrder.m
//  LearnOC
//
//  Created by laderlyoung on 2022/10/10.
//

#import "DeallocOrderTest.h"

@interface DeallocLogger : NSObject

@property(nonatomic, copy) NSString *name;

@end

@implementation DeallocLogger

- (instancetype)initWithName:(NSString *) name {
    self = [super init];
    if (self) {
        self.name = name;
    }
    return self;
}

- (void)dealloc {
    NSLog(@"@DeallocOrder dealloc:%@", self.name);
}

@end

@interface DeallocOrderBaseClass : NSObject

@property(nonatomic, strong) DeallocLogger *base1;
@property(nonatomic, strong) DeallocLogger *base2;

@end


@interface DeallocOrderChildClass : DeallocOrderBaseClass

@property(nonatomic, strong) DeallocLogger *child1;
@property(nonatomic, strong) DeallocLogger *child2;

@end

@implementation DeallocOrderBaseClass

- (instancetype)init {
    if (self = [super init]) {
        self.base1 = [[DeallocLogger alloc] initWithName:@"base-1"];
        self.base2 = [[DeallocLogger alloc] initWithName:@"base-2"];
    }
    return self;
}

- (void)dealloc {
    NSLog(@"@DeallocOrder dealloc DeallocOrderBaseClass");
}

@end


@implementation DeallocOrderChildClass

- (instancetype)init {
    if (self = [super init]) {
        self.child1 = [[DeallocLogger alloc] initWithName:@"child-1"];
        self.child2 = [[DeallocLogger alloc] initWithName:@"child-2"];
    }
    return self;
}

- (void)dealloc {
    NSLog(@"@DeallocOrder dealloc DeallocOrderChildClass");
}
@end

@implementation DeallocOrderTest

+ (void)test {
    NSLog(@"DeallocOrder begin");
    DeallocOrderChildClass *child = [[DeallocOrderChildClass alloc] init];
    child = nil;
    NSLog(@"DeallocOrder end");
}

@end
