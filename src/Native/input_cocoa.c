#include "config.h"
#ifdef USE_COCOA
#include <stdio.h>
#include "menu.h"

#import <Cocoa/Cocoa.h>
#import "ReveryButtonTarget.h"
#import "ReveryNSViewCoords.h"

static ReveryButtonTarget *buttonTarget;

NSButton *revery_buttonCreate_cocoa(const char *title) {
    if (buttonTarget == NULL) {
        buttonTarget = [[ReveryButtonTarget alloc] init];
    }

    NSString *nsTitle = [NSString stringWithUTF8String:title];
    NSButton *nsButton =
        [NSButton buttonWithTitle:nsTitle target:buttonTarget action:@selector(onButtonClick:)];

    [nsButton setReveryX:0.0];
    [nsButton setReveryY:0.0];

    return nsButton;
}

void revery_buttonSetColor_cocoa(NSButton *nsButton, double red, double green, double blue, double alpha) {
    NSColor *color = [NSColor colorWithCalibratedRed:red green:green blue:blue alpha:alpha];

    NSMutableAttributedString *styledTitle = [[NSMutableAttributedString alloc] initWithAttributedString:[nsButton attributedTitle]];
    NSRange titleRange = NSMakeRange(0, [styledTitle length]);
    [styledTitle addAttribute:NSForegroundColorAttributeName value:color range:titleRange];

    [nsButton setAttributedTitle:styledTitle];
}

void revery_openEmojiPanel_cocoa() {
    [NSApp orderFrontCharacterPalette:nil];
}

#endif
