#include "config.h"
#ifdef USE_COCOA
#import <Cocoa/Cocoa.h>
#import <objc/runtime.h>
#import "ReveryNSViewCoords.h"
#include "utilities.h"

@implementation NSView (ReveryNSViewCoords)

CATEGORY_PROPERTY_GET_SET_DOUBLE(reveryX, setReveryX:);
CATEGORY_PROPERTY_GET_SET_DOUBLE(reveryY, setReveryY:);

- (void)updateFrame:(CGRect) frame {
    NSView *superview = [self superview];
    if (superview == NULL) {
        return;
    }

    CGRect superFrame = [superview frame];

    frame.origin.x = [self reveryX];
    frame.origin.y = superFrame.size.height - frame.size.height - [self reveryY];

    [self setFrame:frame];
}

@end

#endif
