#ifdef __APPLE__
#include <stdio.h>

#import <Cocoa/Cocoa.h>
#import "ReveryProgressBar.h"

void *revery_getIconHandle_cocoa() {
    NSDockTile *dock_tile = [NSApp dockTile];
    return dock_tile;
}

NSProgressIndicator *get_progress_indicator(NSDockTile *dock_tile) {
    // Determine if there is already a progress indicator attached to the dock icon
    int first_render = !dock_tile.contentView ||
                       [[dock_tile.contentView subviews] count] == 0 ||
                       ![[[dock_tile.contentView subviews] lastObject]
                                                           isKindOfClass:[NSProgressIndicator class]];

    NSProgressIndicator *progress_indicator;

    if (first_render) {
        NSRect frame = NSMakeRect(0.0f, 0.0f, dock_tile.size.width, 15.0f);
        progress_indicator = [[ReveryProgressBar alloc] initWithFrame:frame];
        [progress_indicator setBezeled:YES];
        [progress_indicator setMinValue:0];
        [progress_indicator setMaxValue:1];
        [progress_indicator setHidden:NO];
        [dock_tile.contentView addSubview:progress_indicator];
    } else {
        progress_indicator = (NSProgressIndicator *)[[[dock_tile contentView] subviews] lastObject];
    }
    return progress_indicator;
}

void revery_setIconProgress_cocoa(void *dt, double progress) {
    NSDockTile *dock_tile = (NSDockTile *)dt;

    NSProgressIndicator *progress_indicator = get_progress_indicator(dock_tile);
    [progress_indicator setIndeterminate:NO];
    [progress_indicator setDoubleValue:progress];
    [progress_indicator setHidden:NO];
    [dock_tile display];
}

void revery_setIconProgressIndeterminate_cocoa(void *dt) {
    NSDockTile *dock_tile = (NSDockTile *)dt;

    NSProgressIndicator *progress_indicator = get_progress_indicator(dock_tile);
    [progress_indicator setIndeterminate:YES];
    [progress_indicator setDoubleValue:1];
    [progress_indicator setHidden:NO];
    [progress_indicator setUsesThreadedAnimation:YES];
    [progress_indicator startAnimation:progress_indicator];
    [dock_tile display];
}

void revery_hideIconProgress_cocoa(void *dt) {
    NSDockTile *dock_tile = (NSDockTile *)dt;

    NSProgressIndicator *progress_indicator = get_progress_indicator(dock_tile);
    [progress_indicator setHidden:YES];
    [progress_indicator release];
    [[NSApp dockTile] display];
}

#endif