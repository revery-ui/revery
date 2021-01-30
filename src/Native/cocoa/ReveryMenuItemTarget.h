#include "config.h"
#ifdef USE_COCOA
#import <Cocoa/Cocoa.h>

@interface ReveryMenuItemTarget : NSObject

-(void)onMenuItemClick:(NSMenuItem *)sender;

@end
#endif
