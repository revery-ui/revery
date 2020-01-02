#ifdef __APPLE__
#include <stdio.h>

#import <Cocoa/Cocoa.h>

void *revery_getIconHandle_cocoa() {
    NSDockTile *dock_tile = [NSApp dockTile];
    return dock_tile;
}

void revery_setIconProgress_cocoa(void *dt, double progress) {
    NSDockTile *dock_tile = (NSDockTile *)dt;

    // Determine if there is already a progress indicator attached to the dock icon
    int first_render = !dock_tile.contentView ||
                       [[dock_tile.contentView subviews] count] == 0 ||
                       ![[[dock_tile.contentView subviews] lastObject]
                                                           isKindOfClass:[NSProgressIndicator class]];

    NSProgressIndicator *progress_indicator;

    if (first_render) {
        NSRect frame = NSMakeRect(0.0f, 0.0f, dock_tile.size.width, 15.0f);
        progress_indicator = [[NSProgressIndicator alloc] initWithFrame:frame];
        [progress_indicator setIndeterminate:NO];
        [progress_indicator setBezeled:YES];
        [progress_indicator setMinValue:0];
        [progress_indicator setMaxValue:1];
        [progress_indicator setHidden:NO];
        [dock_tile.contentView addSubview:progress_indicator];
    } else {
        progress_indicator = (NSProgressIndicator *)[[[dock_tile contentView] subviews] lastObject];
    }

    [progress_indicator setDoubleValue:progress];
    [progress_indicator setHidden:NO];
    [[NSApp dockTile] display];
}

void revery_hideIconProgress_cocoa(void *ip) {
    NSProgressIndicator *progress_indicator = (NSProgressIndicator *)ip;
    [progress_indicator setHidden:YES];
    [progress_indicator release];
    [[NSApp dockTile] display];
}

#endif