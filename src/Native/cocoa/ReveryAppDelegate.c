#ifdef __APPLE__
#import "ReveryAppDelegate.h"
#import <Cocoa/Cocoa.h>

#import "utilities.h"

#define UNUSED(x) (void)(x)

// Implementation of ReveryAppDelegate
@implementation ReveryAppDelegate

/* init - initializes the AppDelegate
  Assigns a mutable dictionary to notificationActions
*/
- (id)init {
  self = [super init];
  if (self) {
    _notificationActions = [NSMutableDictionary new];
  }
  return self;
}

/* applicationDidFinishLaunching
  Assigns self as the notification center delegate
*/
- (void)applicationDidFinishLaunching:(NSNotification *)notification {
  UNUSED(notification);
  [[NSUserNotificationCenter defaultUserNotificationCenter] setDelegate:self];
}

/* didActivateNotification
  Gets the long from the NSDictionary from the notification's identifier and calls it
*/
- (void)userNotificationCenter:(NSUserNotificationCenter *)center
       didActivateNotification:(NSUserNotification *)notification {
  UNUSED(center);
  NSNumber *num = _notificationActions[[notification identifier]];
  long ocamlFunc = [num longValue];
  revery_caml_call(ocamlFunc);
}

/* shouldPresentNotification
  Always presents the notification
*/
- (BOOL)userNotificationCenter:(NSUserNotificationCenter *)center
     shouldPresentNotification:(NSUserNotification *)notification {
  UNUSED(center);
  UNUSED(notification);
  return YES;
}
@end

#endif