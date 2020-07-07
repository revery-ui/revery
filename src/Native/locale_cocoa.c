#include "config.h"
#ifdef USE_COCOA
#include <stdio.h>

#import <Cocoa/Cocoa.h>

char *revery_getUserLocale_cocoa() {
    NSLocale *nsLocale = [NSLocale currentLocale];
    return (char *)[[nsLocale localeIdentifier] UTF8String];
}
#endif
