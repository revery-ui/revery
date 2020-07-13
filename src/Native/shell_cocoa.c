#include "config.h"
#ifdef USE_COCOA
#import <Cocoa/Cocoa.h>

int revery_openURL_cocoa(const char *url_string) {
    NSString *nsString = [NSString stringWithCString:url_string encoding:NSUTF8StringEncoding];
    NSString *nsEncodedString = [nsString stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLFragmentAllowedCharacterSet]];
    NSURL *nsURL = [NSURL URLWithString:nsEncodedString];

    int success = (int)[[NSWorkspace sharedWorkspace] openURL:nsURL];

    [nsEncodedString release];
    [nsString release];
    [nsURL release];

    return success;
}

int revery_openFile_cocoa(const char *path_string) {
    NSString *nsString = [NSString stringWithCString:path_string encoding:NSUTF8StringEncoding];
    NSURL *nsURL = [NSURL fileURLWithPath:nsString];

    int success = (int)[[NSWorkspace sharedWorkspace] openURL:nsURL];

    [nsString release];
    [nsURL release];

    return success;
}

#endif
