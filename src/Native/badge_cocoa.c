#ifdef __APPLE__
#include <stdio.h>

#import <Cocoa/Cocoa.h>

void revery_hideBadge_cocoa() {
    [[NSApp dockTile] setBadgeLabel:NULL];
}

void revery_setBadge_cocoa(char *badgeStr) {
    NSString *nsBadgeStr = [NSString stringWithUTF8String:badgeStr];
    [[NSApp dockTile] setBadgeLabel:nsBadgeStr];
    [nsBadgeStr release];
}

#endif