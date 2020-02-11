type t;

let create: Revery_Core.WindowCreateOptions.t => t;

let render: (t, Revery_UI.element) => unit;

let takeScreenshot: (t, string) => unit;
