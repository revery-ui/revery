/** [t] is the type representing the running app instance

  There is only ever a single app instance, but an app instance
  may have multiple windows.
*/
type t;

type initFunc = t => unit;
type idleFunc = unit => unit;

/** [getWindows(app)] returns the list of all open [Window.t] instances */
let getWindows: t => list(Window.t);

/** [quit(c)] causes the App to quit with exit code [c] */
let quit: int => unit;

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
