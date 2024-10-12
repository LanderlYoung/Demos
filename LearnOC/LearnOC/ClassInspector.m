//
//  ClassInspector.m
//  LearnOC
//
//  Created by landerlyoung on 2021/8/16.
//

#import "ClassInspector.h"
#import <objc/objc-runtime.h>

static inline NSString* NSStringFromBOOL(BOOL aBool) {
    return aBool ? @"YES" : @"NO";
}

static NSString* kIndent = @"  ";

static void inspectIvar(NSMutableString* desc, NSString* baseIndent, unsigned int ivarCount, Ivar const* ivarList);

static void inspectMethod(NSMutableString* desc, NSString* baseIndent, unsigned int methodCount, Method const* methodList);

static void inspectMethodDescriptor(
    NSMutableString* desc,
    NSString* baseIndent,
    NSString* titlePrefix,
    NSString* itemPrefix,
    unsigned int methodCount,
    struct objc_method_description const* methodList);

static void inspectProperty(NSMutableString* desc, NSString* baseIndent, unsigned int propertyCount, objc_property_t const* propertyList);

static void inspectProtocol(NSMutableString* desc, NSString* baseIndent, BOOL instanceMethod, unsigned int protocolCount, __unsafe_unretained Protocol* const* protocolList);

static NSString* makeIndent(int indent) {
    NSMutableString* desc = [[NSMutableString alloc] init];
    for (int i = 0; i < indent; ++i) {
        [desc appendString:kIndent];
    }
    return desc;
}

static NSString* inspectClassInner(Class class, int indent) {
    NSMutableString* desc = [[NSMutableString alloc] init];
    NSString* baseIndent = makeIndent(indent);

    const char* name = class_getName(class);
    BOOL isMetaClass = class_isMetaClass(class);
    size_t instanceSize = class_getInstanceSize(class);

    unsigned int methodCount = 0;
    Method* methodList = class_copyMethodList(class, &methodCount);

    unsigned int ivarCount = 0;
    Ivar* ivarList = class_copyIvarList(class, &ivarCount);

    unsigned int propertyCount = 0;
    objc_property_t* propertyList = class_copyPropertyList(class, &propertyCount);

    unsigned int protocolCount = 0;
    __unsafe_unretained Protocol** protocolList = class_copyProtocolList(class, &protocolCount);

    // class info header
    [desc appendFormat:@"%@<<%s>> meta:%@ size:%@ ivars:%@ method:%@ properties:%@ protocols:%@",
                       baseIndent, name, NSStringFromBOOL(isMetaClass), @(instanceSize),
                       @(ivarCount), @(methodCount), @(propertyCount), @(protocolCount)
    ];

    inspectIvar(desc, baseIndent, ivarCount, ivarList);
    inspectMethod(desc, baseIndent, methodCount, methodList);
    inspectProperty(desc, baseIndent, propertyCount, propertyList);
    inspectProtocol(desc, baseIndent, !isMetaClass, protocolCount, protocolList);

    free(ivarList);
    free(propertyList);
    free(protocolList);
    free(methodList);


    if (!isMetaClass) {
        Class meta = object_getClass(class);
        [desc appendFormat:@"\n%@MetaClass:%@\n%@",
                           baseIndent, kIndent, inspectClassInner(meta, indent + 1)];

        Class super = class_getSuperclass(class);
        if (super) {
            [desc appendFormat:@"\n%@SuperClass:%@\n%@",
                               baseIndent, kIndent, inspectClassInner(super, indent + 1)];
        }
    }
    return desc;
}

static void inspectProtocol(
    NSMutableString* desc,
    NSString* baseIndent,
    BOOL instanceMethod,
    unsigned int protocolCount,
    __unsafe_unretained Protocol* const* protocolList) {

    if (protocolCount > 0) {
        [desc appendFormat:@"\n%@Protocol[%u]:", baseIndent, protocolCount];
        for (size_t i = 0; i < protocolCount; i++) {
            Protocol* proto = protocolList[i];
            const char* name = protocol_getName(proto);
            unsigned int propertyCount = 0;
            objc_property_t* propertyList = nil;
            if (instanceMethod) {
                propertyList = protocol_copyPropertyList(proto, &propertyCount);
            }

            unsigned int methodCount = 0;
            struct objc_method_description* methodList = protocol_copyMethodDescriptionList(
                proto,
                YES,
                instanceMethod,
                &methodCount);

            unsigned int optionalMethodCount = 0;
            struct objc_method_description* optionalMethodList = protocol_copyMethodDescriptionList(
                proto,
                NO,
                instanceMethod,
                &optionalMethodCount);

            if (instanceMethod) {
                [desc appendFormat:@"\n%@%@protocol[%ld] %s method:%@ properties:%@ ",
                                   baseIndent, kIndent, i, name,
                                   @(methodCount + optionalMethodCount),
                                   @(propertyCount)];
                inspectProperty(
                    desc,
                    [baseIndent stringByAppendingFormat:@"%@%@", kIndent, kIndent],
                    propertyCount,
                    propertyList);
            } else {
                [desc appendFormat:@"\n%@%@protocol[%ld] %s method:%@",
                                   baseIndent, kIndent, i, name,
                                   @(methodCount + optionalMethodCount)
                ];
            }
            inspectMethodDescriptor(
                desc,
                [baseIndent stringByAppendingFormat:@"%@%@", kIndent, kIndent],
                instanceMethod ? @"Method" : @"+Method",
                instanceMethod ? @"method" : @"+method",
                methodCount,
                methodList
            );

            inspectMethodDescriptor(
                desc,
                [baseIndent stringByAppendingFormat:@"%@%@", kIndent, kIndent],
                instanceMethod ? @"OptionalMethod" : @"+OptionalMethod",
                instanceMethod ? @"@optional method" : @"+@optional method",
                optionalMethodCount,
                optionalMethodList
            );

            free(propertyList);
            free(methodList);
        }
    }
}

static void inspectProperty(NSMutableString* desc, NSString* baseIndent, unsigned int propertyCount, objc_property_t const* propertyList) {
    if (propertyCount > 0) {
        [desc appendFormat:@"\n%@Property[%u]:", baseIndent, propertyCount];
        for (size_t i = 0; i < propertyCount; i++) {
            objc_property_t prop = propertyList[i];

            const char* props = property_getAttributes(prop);
            const char* name = property_getName(prop);
            [desc appendFormat:@"\n%@%@property[%ld] %s: %s",
                               baseIndent, kIndent, i, name, props];
        }
    }
}

static void inspectMethod(NSMutableString* desc, NSString* baseIndent, unsigned int methodCount, Method const* methodList) {
    if (methodCount > 0) {
        [desc appendFormat:@"\n%@Methods[%u]:", baseIndent, methodCount];
        for (size_t i = 0; i < methodCount; i++) {
            Method method = methodList[i];
            SEL selector = method_getName(method);
            const char* selName = sel_getName(selector);
            IMP imp = method_getImplementation(method);

            const char* typeEncoding = method_getTypeEncoding(method);

            // for type encoding @encode
            // https://gcc.gnu.org/onlinedocs/gcc-6.1.0/gcc/Type-encoding.html
            // https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/ObjCRuntimeGuide/Articles/ocrtTypeEncodings.html#//apple_ref/doc/uid/TP40008048-CH100
            // NSMethodSignature* sig = [NSMethodSignature signatureWithObjCTypes: typeEncoding];

            [desc appendFormat:@"\n%@%@method[%ld] %s %s %p",
                               baseIndent, kIndent, i,
                               selName, typeEncoding, imp
            ];
        }
    }
}

static void inspectMethodDescriptor(
    NSMutableString* desc,
    NSString* baseIndent,
    NSString* titlePrefix,
    NSString* itemPrefix,
    unsigned int methodCount,
    struct objc_method_description const* methodList) {
    if (methodCount > 0) {
        [desc appendFormat:@"\n%@%@[%u]:", baseIndent, titlePrefix, methodCount];
        for (size_t i = 0; i < methodCount; i++) {
            struct objc_method_description method = methodList[i];
            const char* selName = sel_getName(method.name);
            const char* typeEncoding = method.types;

            [desc appendFormat:@"\n%@%@%@[%ld] %s %s",
                               baseIndent, kIndent, itemPrefix, i,
                               selName, typeEncoding
            ];
        }
    }
}


static void inspectIvar(NSMutableString* desc, NSString* baseIndent, unsigned int ivarCount, Ivar const* ivarList) {
    if (ivarCount > 0) {
        [desc appendFormat:@"\n%@Ivars[%u]:", baseIndent, ivarCount];
        for (size_t i = 0; i < ivarCount; i++) {
            Ivar ivar = ivarList[i];
            [desc appendFormat:@"\n%@%@ivar[%ld] %@:%s %s",
                               baseIndent, kIndent, i,
                               @(ivar_getOffset(ivar)),
                               ivar_getName(ivar),
                               ivar_getTypeEncoding(ivar)
            ];
        }
    }
}


@implementation NSObject (ClassInspector)

- (NSString*)inspect {
    NSMutableString* desc = [[NSMutableString alloc] init];
    Class class = [self class];
    [desc appendFormat:@"instance %p is typeof %s", (__bridge void*) self, class_getName(class)];
    id classInspectMessage = [NSObject inspectClass:class];
    [desc appendFormat:@"\n%@", classInspectMessage];
    return desc;
}

+ (NSString*)inspectClass:(Class)class {
    return inspectClassInner(class, 0);
}
@end
