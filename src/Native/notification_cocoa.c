#ifdef __APPLE__
#import "cocoa/ReveryAppDelegate.h"
#import <Cocoa/Cocoa.h>

#include "utilities.h"

void revery_dispatchNotification_cocoa(const char *title, const char *body,
                                       long onClickFunc, int mute) {
    NSUserNotification *notification = [[NSUserNotification alloc] autorelease];
    NSUserNotificationCenter *notificationCenter =
        [NSUserNotificationCenter defaultUserNotificationCenter];
    ReveryAppDelegate *delegate = (ReveryAppDelegate *)[NSApp delegate];
    NSString *nsTitle =
        [NSString stringWithCString:title encoding:NSUTF8StringEncoding];
    NSString *nsBody =
        [NSString stringWithCString:body encoding:NSUTF8StringEncoding];

    NSString *identifier =
        [NSString stringWithFormat:@"%@:notification:%@",
                  [[NSBundle mainBundle] bundleIdentifier],
                  [[[NSUUID alloc] init] UUIDString]];
    [notification setIdentifier:identifier];
    [notification setTitle:nsTitle];
    [notification setInformativeText:nsBody];
    [notification setSoundName:mute ? NULL : NSUserNotificationDefaultSoundName];

    delegate.notificationActions[identifier] =
        [NSNumber numberWithLong:onClickFunc];

    [notificationCenter deliverNotification:notification];
}

void revery_scheduleNotificationFromNow_cocoa(const char *title, const char *body,
        long onClickFunc, int mute, int seconds) {
    NSUserNotification *notification = [[NSUserNotification alloc] autorelease];
    NSUserNotificationCenter *notificationCenter =
        [NSUserNotificationCenter defaultUserNotificationCenter];
    ReveryAppDelegate *delegate = (ReveryAppDelegate *)[NSApp delegate];
    NSString *nsTitle =
        [NSString stringWithCString:title encoding:NSUTF8StringEncoding];
    NSString *nsBody =
        [NSString stringWithCString:body encoding:NSUTF8StringEncoding];

    NSString *identifier =
        [NSString stringWithFormat:@"%@:notification:%@",
                  [[NSBundle mainBundle] bundleIdentifier],
                  [[[NSUUID alloc] init] UUIDString]];
    [notification setIdentifier:identifier];
    [notification setTitle:nsTitle];
    [notification setInformativeText:nsBody];
    [notification setSoundName:mute ? NULL : NSUserNotificationDefaultSoundName];
    NSDate *deliveryDate = [[NSDate alloc] initWithTimeIntervalSinceNow:seconds];
    [notification setDeliveryDate:deliveryDate];

    delegate.notificationActions[identifier] =
        [NSNumber numberWithLong:onClickFunc];

    [notificationCenter scheduleNotification:notification];
}

#endif