#ifdef __APPLE__
#import <Cocoa/Cocoa.h>

int revery_openItemAtURL_cocoa(const char *url_string) {
    NSString *nsString = [NSString stringWithCString:url_string encoding:NSUTF8StringEncoding];

    // This is required for non "file://"-prefixed URLs to work
    NSURL *nsURL;
    if([nsString characterAtIndex:0] == '/') {
        nsURL = [NSURL fileURLWithPath:nsString];
    } else {
        NSString *nsEncodedString = [nsString stringByAddingPercentEncodingWithAllowedCharacters:[NSCharacterSet URLFragmentAllowedCharacterSet]];
        nsURL = [NSURL URLWithString:nsEncodedString];
    }

    return (int)[[NSWorkspace sharedWorkspace] openURL:nsURL];
}

#endif
