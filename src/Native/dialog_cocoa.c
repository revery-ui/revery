#ifdef __APPLE__
#include <stdio.h>

#import <Cocoa/Cocoa.h>

@interface CustomWindowButtonView : NSView {
    @private
        BOOL mouse_inside_;
}
@end

@implementation CustomWindowButtonView

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    NSButton* close_button = [NSWindow standardWindowButton:NSWindowCloseButton forStyleMask:NSWindowStyleMaskTitled];
    [close_button setTag:1];

    NSButton* miniaturize_button = [NSWindow standardWindowButton:NSWindowMiniaturizeButton forStyleMask:NSWindowStyleMaskTitled];
    [miniaturize_button setTag:2];

    NSButton* zoom_button = [NSWindow standardWindowButton:NSWindowZoomButton forStyleMask:NSWindowStyleMaskTitled];
    [zoom_button setTag:3];

    CGFloat x = 0;
    const CGFloat space_between = 20;
    [close_button setFrameOrigin:NSMakePoint(x, 0)];
    x += space_between;
    [self addSubview:close_button];

    [miniaturize_button setFrameOrigin:NSMakePoint(x, 0)];
    x += space_between;
    [self addSubview:miniaturize_button];

    [zoom_button setFrameOrigin:NSMakePoint(x, 0)];
    x += space_between;
    [self addSubview:zoom_button];

    const NSRect last_button_frame = zoom_button.frame;
    [self setFrameSize:NSMakeSize(last_button_frame.origin.x + last_button_frame.size.width, last_button_frame.size.height)];

    /*[self setFrameOrigin:NSMakePoint(10, 10)];*/
    mouse_inside_ = YES;
    [self setNeedsDisplayForButtons];

    return self;
}

- (BOOL)_mouseInGroup:(NSButton *)button {
    return mouse_inside_;
}

- (void)viewDidMoveToWindow {
    if (!self.window) {
        return;
    }

    const CGFloat top_margin = 3;
    const CGFloat left_margin = 7;

    [self setAutoresizingMask:NSViewMaxXMargin | NSViewMinYMargin];
    [self setFrameOrigin:NSMakePoint(left_margin, self.window.frame.size.height - self.frame.size.height - top_margin)];
}

- (void)updateTrackingAreas {
    NSTrackingArea* tracking_area = [[NSTrackingArea alloc]
        initWithRect:NSZeroRect
            options:(NSTrackingMouseEnteredAndExited | NSTrackingActiveAlways | NSTrackingInVisibleRect)
            owner:self
            userInfo:nil]; 
    [self addTrackingArea:tracking_area];
}

- (void)mouseEntered:(NSEvent*)event {
    [super mouseEntered:event];
    mouse_inside_ = YES;
    [self setNeedsDisplayForButtons];
}

- (void)mouseExited:(NSEvent*)event {
    [super mouseExited:event];
    mouse_inside_ = NO;
    [self setNeedsDisplayForButtons];
}

- (void)setNeedsDisplayForButtons {
   for (NSView* subview in self.subviews) {
     /*[subview setHidden:!mouse_inside_];*/
     [subview setNeedsDisplay:YES];
   } 
}

@end

void revery_alert_cocoa(void *pWin, const char *szMessage) {
    NSWindow* pCocoaWin  = (NSWindow *)pWin;

    NSUInteger styleMask = 0;
    styleMask |= NSWindowStyleMaskBorderless;
    styleMask |= NSWindowStyleMaskResizable;

    /*pCocoaWin.title = @"TEST";
    pCocoaWin.titlebarAppearsTransparent = false;
        */

    [pCocoaWin setStyleMask:styleMask];
    /*[[pCocoaWin standardWindowButton:NSWindowCloseButton] setHidden:YES];*/

    NSView *buttonsView = [[CustomWindowButtonView alloc] initWithFrame:CGRectMake(100, 100, 100, 100)];
    [buttonsView setWantsLayer:YES];
        [[pCocoaWin contentView] addSubview:buttonsView];

        NSView *superview = [pCocoaWin contentView];
    NSRect frame = NSMakeRect(10, 10, 200, 200);
    NSButton *button = [[NSButton alloc] initWithFrame:frame];
    [button setWantsLayer:YES];
    [button setTitle:@"Click me!"];
    /*[superview addSubview:button positioned:NSWindowAbove relativeTo:nil]; */
    [button release];

    /*[pCocoaWin makeFirstResponder:pCocoaWin.contentView];*/
    /*NSView *view = [[NSView alloc] init];
    NSAlert *alert = [[NSAlert alloc] init];
    NSString *message = [NSString stringWithUTF8String:szMessage];
    [alert addButtonWithTitle:@"Ok"];
    [alert setMessageText:@"Alert"];
    [alert setInformativeText:message];
    [alert runModal];*/
}
#endif
