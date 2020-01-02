#ifdef __APPLE__
#import <Cocoa/Cocoa.h>
#import "ReveryProgressBar.h"

@implementation ReveryProgressBar

- (void)drawRect:(NSRect)dirtyRect {
    (void)dirtyRect;
    NSRect rect = NSInsetRect([self bounds], 1.0, 1.0);
    CGFloat radius = rect.size.height / 2;
    NSBezierPath* bezier_path = [NSBezierPath bezierPathWithRoundedRect:rect
                                              xRadius:radius
                                              yRadius:radius];
    [bezier_path setLineWidth:2.0];
    [[NSColor grayColor] set];
    [bezier_path stroke];

    rect = NSInsetRect(rect, 2.0, 2.0);
    radius = rect.size.height / 2;
    bezier_path = [NSBezierPath bezierPathWithRoundedRect:rect
                                xRadius:radius
                                yRadius:radius];
    [bezier_path setLineWidth:1.0];
    [bezier_path addClip];

    rect.size.width =
        floor(rect.size.width * ([self doubleValue] / [self maxValue]));
    if (self.indeterminate) {
        [[NSColor colorWithSRGBRed:1 green:0.84 blue:.04 alpha:1] set];
    } else {
        [[NSColor colorWithSRGBRed:0.39 green:0.82 blue:1 alpha:1] set];
    }
    NSRectFill(rect);
}

@end
#endif