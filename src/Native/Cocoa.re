/* Cocoa.re
     All macOS specific code, primarily setting the custom AppDelegate
   */
%import
"config.ml";

// If we're on a Mac, deine the functions properly
%if
operating_system == "Darwin";

external setAppDelegate: unit => unit = "revery_cocoaSetAppDelegate";

[%%else]
// If we're _not_ on a Mac, define everything as a noop

let _noop = () => ();
let setAppDelegate = _noop;

[%%endif];

