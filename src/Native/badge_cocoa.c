#ifdef __APPLE__
#include <stdio.h>

#import <Cocoa/Cocoa.h>

void revery_hideBadge_cocoa() {
  [[NSApp dockTile] setBadgeLabel:NULL];
}

void revery_setBadgeNumber_cocoa(char *badgeNumStr) {
  NSString *nsBadgeNumStr = [NSString stringWithUTF8String:badgeNumStr];
  [[NSApp dockTile] setBadgeLabel:nsBadgeNumStr];
  [nsBadgeNumStr release];
}

#endif