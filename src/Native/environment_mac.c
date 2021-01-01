#include "config.h"
#ifdef IS_MACOS
#import <Foundation/Foundation.h>

void getOperatingSystemVersion_mac(int *major, int *minor, int *bugfix) {
    NSOperatingSystemVersion nsOSVersion = [[NSProcessInfo processInfo] operatingSystemVersion];
    *major = nsOSVersion.majorVersion;
    *minor = nsOSVersion.minorVersion;
    *bugfix = nsOSVersion.patchVersion;
}

#endif
