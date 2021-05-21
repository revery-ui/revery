%import
"config.h";

module Button = {
  type t;

  open {
         external c_create: string => t = "revery_buttonCreate";
         external c_setColor: (t, float, float, float, float) => unit =
           "revery_buttonSetColor";
       };

  %if
  defined(USE_COCOA);

  let hash = NSObject.hash;
  let equal = NSObject.equal;
  let toString = NSObject.toString;

  [%%else];

  let hash = _ => 1;
  let equal = (===);
  let toString = _ => "UNIMPLEMENTED";

  [%%endif];

  module CallbackTbl =
    Hashtbl.Make({
      type nonrec t = t;
      let equal = equal;
      let hash = hash;
    });

  let callbackTbl = CallbackTbl.create(32);

  let callbackForButton = button => {
    let callback = CallbackTbl.find_opt(callbackTbl, button);
    switch (callback) {
    | Some(cb) => cb()
    | None => ()
    };
  };

  Callback.register("revery_callbackForButton", callbackForButton);

  let setColor = (~red, ~green, ~blue, ~alpha, button) =>
    c_setColor(button, red, green, blue, alpha);

  %if
  defined(USE_COCOA);

  let create = (~title, ~onClick) => {
    let button = c_create(title);
    CallbackTbl.replace(callbackTbl, button, onClick);
    Gc.finalise(NSObject.release, button);
    button;
  };

  let setFrame = (~x, ~y, ~width, ~height, button) =>
    NSView.setFrame(button, x, y, width, height);

  let getDefaultWidth = NSView.getDefaultWidth;
  let getDefaultHeight = NSView.getDefaultHeight;

  let remove = NSView.remove;
  let displayIn = NSView.displayIn;

  [%%else];

  let create = (~title, ~onClick) => {
    let button = c_create(title);
    CallbackTbl.replace(callbackTbl, button, onClick);
    button;
  };

  let setFrame = (~x as _, ~y as _, ~width as _, ~height as _, _) => ();

  let getDefaultWidth = _ => 0;
  let getDefaultHeight = _ => 0;

  let remove = _ => ();
  let displayIn = (_, _) => ();

  [%%endif];
};

external openEmojiPanel: unit => unit = "revery_openEmojiPanel";
