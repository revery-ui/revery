#ifdef __APPLE__
#include <stdio.h>

#import <Cocoa/Cocoa.h>

void revery_alert_cocoa(void *pWin, const char *szMessage) {
    NSWindow* pCocoaWin  = (NSWindow *)pWin;

    NSView *view = [[NSView alloc] init];
    NSAlert *alert = [[NSAlert alloc] init];
    NSString *message = [NSString stringWithUTF8String:szMessage];
    [alert addButtonWithTitle:@"Ok"];
    [alert setMessageText:@"Alert"];
    [alert setInformativeText:message];
    [alert runModal];
}

const char** revery_open_files_cocoa(const char *startDir, char *fileTypes[], const char *title) {
    NSArray *nsFileTypes = NULL;
    if (fileTypes) {
        int count = 0;
        while (fileTypes[count] != NULL) count++;
        NSMutableArray *tmpArr = [[NSMutableArray alloc] initWithCapacity: count];
        for (int i = 0; i < count; i++) {
            [tmpArr addObject: [NSString stringWithCString: fileTypes[i] encoding: NSUTF8StringEncoding]];
        }
        nsFileTypes = tmpArr;
    }
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    [panel setAllowsMultipleSelection:false];
    [panel setFloatingPanel:YES];
    if (startDir) {
        NSString *urlString = [NSString stringWithCString: startDir encoding: NSUTF8StringEncoding];
        NSLog(urlString);
        NSURL *url = [NSURL fileURLWithPath: urlString];
        [panel setDirectoryURL:url];
    }
    NSInteger result = [panel runModal];

    if (result == NSModalResponseOK) {
        NSArray *urls = [panel URLs];
        int size = [urls count];
        const char *ret[size];
        for (int i = 0; i < size; i++) {
            NSString *tmp = [[urls objectAtIndex:i] path];
            ret[i] = [tmp UTF8String];
        }
        return ret;
    } else {
        return NULL;
    }
}
#endif
