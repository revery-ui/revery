/** [t] is the type representing the running app instance

  There is only ever a single app instance, but an app instance
  may have multiple windows.
*/
type t;

type initFunc = t => unit;
type idleFunc = unit => unit;
type canIdleFunc = unit => bool;

/** [getWindows(app)] returns the list of all open [Window.t] instances */
let getWindows: t => list(Window.t);

/** [quit(c)] causes the App to quit with exit code [c] */
let quit: int => unit;

/** [isIdle(app)] returns true if the app is idling, false othwrise */
let isIdle: t => bool;

type delegatedFunc = unit => unit;

/** [runOnMainThread(f)] schedules the function [f] to run during the next
    render frame on the main thread.

    If the main thread is idle, this will force the main thread to render
*/
let runOnMainThread: delegatedFunc => unit;

/** [setCanIdle(f, app)] registers a callback [f]. Each frame, [f] will be called
 to check if the application can idle. If [f()] returns [false], the app will not
 idle and will render. This is useful if you have animations active, but in general,
 you want to allow the app to idle to minimize CPU and battery usage.
*/
let setCanIdle: (canIdleFunc, t) => unit;

/** [createWindow ~createOptions, app, name] creates a new window */
let createWindow:
  (~createOptions: WindowCreateOptions.t=?, t, string) => Window.t;

/** [start] is the entry point for a Revery application. This initiates
  the Revery application lifecycle, and an app instance ([t]) is passed
  to an initialization function.

  An optional [idleFunc] may be provided if there is work that could be
  done when the application is idle for performance (for example,
  this is a good time to garbage collect). This will be called
  when multiple frames have passed without requiring a render.
*/
let start: (~onIdle: idleFunc=?, initFunc) => unit;
