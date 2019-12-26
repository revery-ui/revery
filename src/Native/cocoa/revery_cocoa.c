#ifdef __APPLE__
#import "ReveryAppDelegate.h"


void revery_cocoaSetAppDelegate() {
  ReveryAppDelegate *delegate = [ReveryAppDelegate new];
  [NSApp setDelegate:delegate];
}
#endif