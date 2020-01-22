#ifdef __APPLE__
#import <Cocoa/Cocoa.h>
#import "SDLAppDelegate.h"

/* ReveryAppDelegate

  Contains all of the delegation code for the NSApplication, as well as the
    delegation for the NSUserNotificationCenter
*/
@interface ReveryAppDelegate
    : NSObject <NSApplicationDelegate, NSUserNotificationCenterDelegate>
      /* [notificationActions] - a mutable dictionary
        maps NSStrings (notification identifiers) -> NSNumbers
          (longs which represent OCaml callbacks)
      */
  @property(nonatomic, strong) NSMutableDictionary *notificationActions;
@property(nonatomic, strong) SDLAppDelegate *sdlDelegate;

-(id)initWithSDLDelegate:(SDLAppDelegate *)sdlDelegate;
+(id)newWithSDLDelegate:(SDLAppDelegate *)sdlDelegate;
@end
#endif