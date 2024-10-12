//
//  DeallocOrder.m
//  LearnOC
//
//  Created by laderlyoung on 2022/10/10.
//

#import "DeallocOrder.h"

@implementation DeallocOrder

- (void)dealloc {
    NSLog(@"dealloc %@ %@", self.name, [self description]);
}

+ (void)test {
    DeallocOrder *p = [[DeallocOrder alloc] init];
    p.name = @"parent";
    p.obj = ({
        DeallocOrder *c = [[DeallocOrder alloc] init];
        c.name = @"child";
        c;
    });
    
    p = nil;
}

@end
