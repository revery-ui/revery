%import
"config.h";

module Button = {
  type t;

  open {
         external c_create: string => t = "revery_buttonCreate";
       };

  %if
  defined(USE_COCOA);

  let hash = NSObject.hash;
  let equal = NSObject.equal;
  let toString = NSObject.toString;

  [%%else];

  let hash = _ => 1;
  let equal = (==);
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

  %if
  defined(USE_COCOA);

  let create = (~title, ~onClick) => {
    let button = c_create(title);
    CallbackTbl.replace(callbackTbl, button, onClick);
    Gc.finalise(NSObject.release, button);
    button;
  };

  let setX = NSView.setX;
  let setY = NSView.setY;
  let getX = NSView.getX;
  let getY = NSView.getY;

  let setWidth = NSView.setWidth;
  let setHeight = NSView.setHeight;
  let getWidth = NSView.getWidth;
  let getHeight = NSView.getHeight;
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

  let setX = (_, _) => ();
  let setY = (_, _) => ();
  let getX = _ => 0;
  let getY = _ => 0;

  let setWidth = (_, _) => ();
  let setHeight = (_, _) => ();
  let getWidth = _ => 0;
  let getHeight = _ => 0;
  let getDefaultWidth = _ => 0;
  let getDefaultHeight = _ => 0;

  let remove = _ => ();
  let displayIn = (_, _) => ();

  [%%endif];
};
