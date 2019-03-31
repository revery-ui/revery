open Js_of_ocaml;
open Js_of_ocaml_toplevel;

open Revery;
open Revery.UI;

open Js_of_ocaml;

open PlaygroundLib.Types;

let execute: Js.t(Js.js_string) => Js.t(Js.js_string) = code => {
  let code = Js.to_string(code);
  let buffer = Buffer.create(100);
  let formatter = Format.formatter_of_buffer(buffer);
  JsooTop.execute(true, formatter, code);
  let result = Buffer.contents(buffer)
  Js.string(result);
};

let postfix = "\nPlaygroundLib.Worker.setRenderFunction(render);";

let execute2: Js.t(Js.js_string) => Js.t(Js.js_string) = code => {
  let code = Js.to_string(code) ++ postfix;
  let buffer = Buffer.create(100);
  let formatter = Format.formatter_of_buffer(buffer);
  JsooTop.execute(true, formatter, code);
  let result = Buffer.contents(buffer)
  Js.string(result);
};

let reasonSyntax = () => {
  open Reason_toolchain.From_current;
  let wrap = (f, g, fmt, x) => g(fmt, f(x));
  Toploop.parse_toplevel_phrase :=
    Reason_util.correctly_catch_parse_errors(x =>
      Reason_toolchain.To_current.copy_toplevel_phrase(
        Reason_toolchain.RE.toplevel_phrase(x),
      )
    );
  Toploop.parse_use_file :=
    Reason_util.correctly_catch_parse_errors(x =>
      List.map(
        Reason_toolchain.To_current.copy_toplevel_phrase,
        Reason_toolchain.RE.use_file(x),
      )
    );
  Toploop.print_out_value :=
    wrap(copy_out_value, Reason_oprint.print_out_value);
  Toploop.print_out_type := wrap(copy_out_type, Reason_oprint.print_out_type);
  Toploop.print_out_class_type :=
    wrap(copy_out_class_type, Reason_oprint.print_out_class_type);
  Toploop.print_out_module_type :=
    wrap(copy_out_module_type, Reason_oprint.print_out_module_type);
  Toploop.print_out_type_extension :=
    wrap(copy_out_type_extension, Reason_oprint.print_out_type_extension);
  Toploop.print_out_sig_item :=
    wrap(copy_out_sig_item, Reason_oprint.print_out_sig_item);
  Toploop.print_out_signature :=
    wrap(List.map(copy_out_sig_item), Reason_oprint.print_out_signature);
  Toploop.print_out_phrase :=
    wrap(copy_out_phrase, Reason_oprint.print_out_phrase);
};

let derp = () => print_endline("Hello, world!");

let renderFunction = ref(() => <View style={Style.[backgroundColor(Colors.red), width(100), height(100)]} />);

let _pendingUpdates: ref(list(updates)) = ref([]);
let clearUpdates = () => _pendingUpdates := [];
let queueUpdate = (update: updates) => {
   _pendingUpdates := [update, ..._pendingUpdates^]; 
};

class proxyViewNode (()) = {
    as _this;
    inherit (class viewNode)() as super;

    pub! setStyle = style => {
      queueUpdate(SetStyle(super#getInternalId(), style));
    };

    pub! addChild = (child) => {
        print_endline ("WORKER: ADD CHILD");
        queueUpdate(AddChild(super#getInternalId(), child#getInternalId()));   
        super#addChild(child);
    };

    pub! removeChild = (child) => {
        print_endline ("WORKER: REMOVE CHILD");
        queueUpdate(RemoveChild(super#getInternalId(), child#getInternalId()));   
        super#removeChild(child);
    };
    
    initializer {
        queueUpdate(NewNode(super#getInternalId(), View));
    }
}

let rootNode = (new proxyViewNode)();
queueUpdate(RootNode(rootNode#getInternalId()));
let container = ref(Container.create(rootNode));




let proxyNodeFactory: nodeFactory = {
   createViewNode: () => (new proxyViewNode)(), 
};

setNodeFactory(proxyNodeFactory);


let setRenderFunction = (fn) => {
    /* print_endline ("setting render function4"); */
    /* let testVal = fn(); */
    /* let marshalledData = Marshal.to_string(testVal, [Marshal.Closures]); */
    /* print_endline ("Marshalled data: " ++ marshalledData); */
    /* let unmarshalledData = Obj.magic(Marshal.from_string(marshalledData)); */

    renderFunction := fn;

    print_endline ("before render function - childCount: " ++ string_of_int(List.length(rootNode#getChildren())));
    container := Container.update(container^, fn());
    print_endline ("Set render function - childCount: " ++ string_of_int(List.length(rootNode#getChildren())));

    print_endline("Trying to post...");
    /* let _derp = Js.string("hi"); */
    let updatesToSend = _pendingUpdates^ |> List.rev;
    PlaygroundLib.Types.showAll(updatesToSend);
    Worker.post_message(updatesToSend);
    print_endline("Posted!" ++ string_of_int(List.length(updatesToSend)));
    clearUpdates();

};

let log = (v) => print_endline ("[Worker] " ++ v);

let _ = {
  reasonSyntax();
  JsooTop.initialize();

  Worker.set_onmessage((updates: PlaygroundLib.Protocol.ToWorker.t) => {
      log ("WORKER: GOT UPDATES");
      switch (updates) {
      | SourceCodeUpdated(v) => {
          log ("got source code update");
          let _  = execute(v);
      }
    | _ => log("unknown update");
    }
  });

  log("Initialized");
  /* PlaygroundLib.startPlayground(); */
};

