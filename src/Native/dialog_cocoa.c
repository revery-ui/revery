#include "config.h"
#ifdef USE_COCOA
#include <stdio.h>

#import <Cocoa/Cocoa.h>
#import "ReveryProgressBar.h"


void revery_alert_cocoa(void *pWin, const char *szMessage) {
    (void)pWin;

    NSAlert *alert = [[NSAlert alloc] init];
    NSString *message = [NSString stringWithUTF8String:szMessage];
    [alert addButtonWithTitle:@"Ok"];
    [alert setMessageText:@"Alert"];
    [alert setInformativeText:message];
    [alert runModal];
    [alert release];
    [message release];
}

const char **revery_open_files_cocoa(
    const char *startDir, const char *fileTypes[], int fileTypesSize,
    int allowMultiple, int canChooseFiles, int canChooseDirectories,
    int showHidden, const char *buttonText, const char *title) {

    // Grab the key window, so we can restore focus after dialog..
    NSWindow *keyWindow = [NSApp keyWindow];
    /* Creates an empty NSArray of filetypes (NSString's)
        If [fileTypes] is not null, copy the C-strings to NSString's to the
       NSArray
    */
    NSArray *nsFileTypes = NULL;
    if (fileTypes) {
        NSMutableArray *tmpArr =
            [[NSMutableArray alloc] initWithCapacity:fileTypesSize];
        for (int i = 0; i < fileTypesSize; i++) {
            // Convert char* -> NSString
            NSString *str = [NSString stringWithCString:fileTypes[i]
                                      encoding:NSUTF8StringEncoding];
            [tmpArr addObject:str];
        }
        nsFileTypes = tmpArr;
    }
    // The actual dialog itself
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    // We can directly set these because it will either be 0 or 1
    [panel setAllowsMultipleSelection:allowMultiple];
    [panel setCanChooseFiles:canChooseFiles];
    [panel setCanChooseDirectories:canChooseDirectories];
    [panel setShowsHiddenFiles:showHidden];

    if (startDir) {
        // If [startDir] is not NULL, convert it to an NSString...
        NSString *urlString =
            [NSString stringWithCString:startDir encoding:NSUTF8StringEncoding];
        // ...and then to an NSURL
        NSURL *url = [NSURL fileURLWithPath:urlString];
        [panel setDirectoryURL:url];
    }

    if (title) {
        [panel setMessage:[NSString stringWithCString:title
                           encoding:NSUTF8StringEncoding]];
        [panel setTitle:[NSString stringWithCString:title
                         encoding:NSUTF8StringEncoding]];
    }

    if (buttonText)
        [panel setPrompt:[NSString stringWithCString:buttonText
                          encoding:NSUTF8StringEncoding]];

    if (nsFileTypes) [panel setAllowedFileTypes:nsFileTypes];

    // Run the actual panel/modal
    NSInteger result = [panel runModal];

    [panel release];
    [nsFileTypes release];

    // If a file(s) was selected...
    if (result == NSModalResponseOK) {
        // ...get the list of URLs
        NSArray *urls = [panel URLs];
        /* Create a C-array with the size + 1 of the NSArray
            We NULL terminate it so we can get the size in the main function
        */
        int size = [urls count];
        int actualSize = 0;
        const char **ret = malloc((size + 1) * sizeof(char *));
        // Copy the NSArray to the C-array
        for (int i = 0; i < size; i++) {
            NSString *tmp = [[urls objectAtIndex:i] path];
            const char *sz= [tmp cStringUsingEncoding:NSUTF8StringEncoding];
            // According to the Objective-C docs, the returned string
            // is only guaranteed to be valid for the lifetime of
            // the [NSString]:
            // https://developer.apple.com/documentation/foundation/nsstring/1408489-cstringusingencoding?language=objc
            // So we need to make a copy here...
            if (sz != NULL) {
                ret[actualSize] = strdup(sz);
                actualSize++;
            }
        }
        [urls release];
        ret[actualSize] = NULL;
        [keyWindow makeKeyWindow];
        return ret;
    } else {
        // ...else return NULL
        [keyWindow makeKeyWindow];
        return NULL;
    }
}
#endif
