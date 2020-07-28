#include "config.h"
#ifdef USE_COCOA
#include <stdio.h>

#import <Cocoa/Cocoa.h>

void *revery_makeTrayHandleText_cocoa(const char *titleText) {
  NSStatusItem *statusItem = [NSStatusBar.systemStatusBar statusItemWithLength:NSVariableStatusItemLength];

  NSString *nsTitle =
      [NSString stringWithCString:titleText encoding:NSUTF8StringEncoding];

  statusItem.button.title = nsTitle;

  return statusItem;
}

void *revery_makeTrayHandleImage_cocoa(const char *titleImage) {
  NSStatusItem *statusItem = [NSStatusBar.systemStatusBar statusItemWithLength:NSVariableStatusItemLength];

  NSString *nsImagePath =
      [NSString stringWithCString:titleImage encoding:NSUTF8StringEncoding];

  NSImage *nsImage = [[NSImage alloc]initWithContentsOfFile:nsImagePath];

  statusItem.button.image = nsImage;

  return statusItem;
}

#endif
