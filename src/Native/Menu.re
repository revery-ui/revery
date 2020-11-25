%import
"config.h";

type t;

module Item = {
  type t;

  %if
  defined(USE_COCOA);

  let hash = item => item |> NSObject.wrap |> NSObject.hash;
  let equal = item => item |> NSObject.wrap |> NSObject.equal;
  let toString = menu => menu |> NSObject.wrap |> NSObject.toString;

  [%%else];

  let hash = _ => 1;
  let equal = (==);

  [%%endif];

  open {
         external c_create: string => t = "revery_createMenuItem";
       };

  module CallbackTbl =
    Hashtbl.Make({
      type nonrec t = t;
      let equal = equal;
      let hash = hash;
    });

  let callbackTbl = CallbackTbl.create(32);

  let callbackForMenuItem = item => {
    let callback = CallbackTbl.find_opt(callbackTbl, item);
    switch (callback) {
    | Some(cb) => cb()
    | None => ()
    };
  };

  Callback.register("revery_callbackForMenuItem", callbackForMenuItem);

  let create = (~title, ~onClick) => {
    let menu = c_create(title);
    CallbackTbl.replace(callbackTbl, menu, onClick);
    menu;
  };
};

external getMenuBarHandle: unit => t = "revery_getMenuBarHandle";
external create: string => t = "revery_createMenu";

%if
defined(USE_COCOA);

let toString = menu => menu |> NSObject.wrap |> NSObject.toString;

[%%else];

let toString = _ => "UNIMPLEMENTED";

[%%endif];
