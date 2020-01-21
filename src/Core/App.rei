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

/**
[quit(~askNicely, ~code, c)] causes the App to quit with exit code [c]

[askNicely] specifies whether quit should be forced (default [false]). If [true],
the canQuit handlers will be run for each window. If [false], the canQuit handlers
will be ignored.

[code] specifies the exit code. Defaults to [0].
*/
let quit: (~askNicely: bool=?, ~code: int=?, t) => unit;

/** [isIdle(app)] returns true if the app is idling, false othwrise */
let isIdle: t => bool;

type delegatedFunc = unit => unit;

/** [runOnMainThread(f)] schedules the function [f] to run during the next
    render frame on the main thread.

    If the application is idle, this will cause it to become active, and trigger a re-render.
*/
let runOnMainThread: delegatedFunc => unit;

/** [flushPendingCallbacks(f)] will explicitly run all the callbacks
    queued up via [runOnMainThread].

    In general, this should not need to be called by user code,
    as it happens as part of the application lifecycle.

    However, it may be necessary to call this for tests that queue
    up callbacks via [runOnMainThread].
*/
let flushPendingCallbacks: unit => unit;

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

/** [initConsole] (Windows-only) attaches or allocates a console,
  to show logging output. No-op on other platforms.
*/
let initConsole: unit => unit;
