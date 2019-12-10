#ifdef __APPLE__
#include <stdio.h>

#import <Cocoa/Cocoa.h>

void *revery_register_icon_progress_cocoa() {
    NSDockTile *dock_tile = [NSApp dockTile];
    NSRect frame = NSMakeRect(0.0f, 0.0f, dock_tile.size.width, 15.0f);
    NSProgressIndicator *progress_indicator =
        [[NSProgressIndicator alloc] initWithFrame:frame];
    [progress_indicator setStyle:NSProgressIndicatorBarStyle];
    [progress_indicator setIndeterminate:NO];
    [progress_indicator setBezeled:YES];
    [progress_indicator setMinValue:0];
    [progress_indicator setMaxValue:1];
    [progress_indicator setHidden:NO];
    [dock_tile.contentView addSubview:progress_indicator];
    return (void *)progress_indicator;
}

void revery_set_icon_progress_cocoa(void *ip, double progress) {
    NSProgressIndicator *progress_indicator = (NSProgressIndicator *)ip;
    [progress_indicator setMinValue:0];
    [progress_indicator setMaxValue:1];
    [progress_indicator setDoubleValue:progress];
    [progress_indicator setHidden:NO];
    [[NSApp dockTile] display];
}

void revery_deregister_icon_progress_cocoa(void *ip) {
    NSProgressIndicator *progress_indicator = (NSProgressIndicator *)ip;
    [progress_indicator setHidden:YES];
    [progress_indicator release];
    [[NSApp dockTile] display];

}

#endif