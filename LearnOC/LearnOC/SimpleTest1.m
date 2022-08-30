//
//  SimpleTest1.m
//  SimpleTest1
//
//  Created by landerlyoung on 2021/9/7.
//

#import "SimpleTest1.h"

@protocol MyProto
-(void) protoMethod:(NSString*) what;
+(void) protoClassMethod:(NSString*) what;

@optional
-(void) optionalProtoMethod:(NSString*) what;
+(void) optionalProtoClassMethod:(NSString*) what;

@end

@interface MyClass : NSObject<MyProto> {
    NSInteger counter;
}

- (void) instanceHello:(NSString*) name age:(NSInteger) age;

@end

@implementation MyClass

- (void) instanceHello:(NSString*) name age:(NSInteger) age {
    NSLog(@"-MyClass hello %@ %ld", name, age);
}

+ (void) classHello:(NSString*) name age:(NSInteger) age {
    NSLog(@"+MyClass hello %@ %ld，self:%p, self.class:%p", name, age, self, self.class);
}

+ (void) initialize {
    NSLog(@"%@ intialize", [[self class] className]);
}

@end

@interface MyClassSub: MyClass

@end

@implementation MyClassSub

@end


@implementation SimpleTest1

+(id) runTest {
    id myClass = [[MyClass alloc] init];
    
    Class class = [myClass class];

    id cc = MyClass.class;
    id ccc = [MyClass class];
    id classOfClass = [cc class];
    assert(cc == ccc);
    assert(class == cc);
    assert(classOfClass == cc);
    
    [myClass instanceHello:@"world" age:42];
    [MyClass classHello:@"world" age:42];
    [MyClassSub classHello:@"world-sub" age:42];
    [cc classHello:@"world" age:42];
    
    return myClass;
}

@end
