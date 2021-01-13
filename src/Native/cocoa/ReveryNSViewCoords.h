#include "config.h"
#ifdef USE_COCOA
#import <Cocoa/Cocoa.h>

/* ReveryNSViewCoords
    Apple broke from the crowd and defines NSView coordinates to be
    relative to the bottom left rather than the top. This is a category
    to "extend" the NSView class (and all subclasses) so that we can store
    "normal" coordinates and update the frame when the NSView has a parent.
*/
@interface NSView (ReveryNSViewCoords)

@property double reveryX;
@property double reveryY;

- (void)updateFrame:(CGRect) frame;

@end

#endif

