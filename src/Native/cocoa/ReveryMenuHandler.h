#include "config.h"
#ifdef USE_COCOA
#import <Cocoa/Cocoa.h>

@interface ReveryMenuHandler : NSObject
@property(nonatomic, strong) NSMutableDictionary *menuActions;

-(void)menuClickHandler:(id)sender;

-(void)registerOnClick:(NSMenuItem *)menuItem callback:(long)camlCallback;

-(void)unregisterOnClick:(NSMenuItem *)menuItem;
@end

#endif
