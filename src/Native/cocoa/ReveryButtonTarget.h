#include "config.h"
#ifdef USE_COCOA
#import <Cocoa/Cocoa.h>

@interface ReveryButtonTarget : NSObject

-(void)onButtonClick:(NSButton *)sender;

@end
#endif
