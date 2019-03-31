open Revery;
open Revery.UI;

open Js_of_ocaml;

open Types;

let derp = () => print_endline("Hello, world!");

let renderFunction = ref(() => <View style={Style.[backgroundColor(Colors.red), width(100), height(100)]} />);

let rootNode = (new viewNode)();

let container = ref(Container.create(rootNode));

let _pendingUpdates: ref(list(updates)) = ref([]);
let clearUpdates = () => _pendingUpdates := [];
let queueUpdate = (update: updates) => {
   _pendingUpdates := [update, ..._pendingUpdates^]; 
};

queueUpdate(NewNode(rootNode#getInternalId(), View));
queueUpdate(RootNode(rootNode#getInternalId()));

class proxyViewNode (()) = {
    as _this;
    inherit (class viewNode)() as super;

    pub setStyle = style => {
      queueUpdate(SetStyle(super#getInternalId(), style));
    };

    pub addChild = (child) => {
        queueUpdate(AddChild(super#getInternalId(), child#getInternalId()));   
        super#addChild(child);
    };

    pub removeChild = (child) => {
        queueUpdate(RemoveChild(super#getInternalId(), child#getInternalId()));   
        super#removeChild(child);
    };
    
    initializer {
        queueUpdate(NewNode(super#getInternalId(), View));
    }
}

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

    container := Container.update(container^, fn());
    print_endline ("Set render function - childCount: " ++ string_of_int(List.length(rootNode#getChildren())));

    print_endline("Trying to post...");
    /* let _derp = Js.string("hi"); */
    let updatesToSend = _pendingUpdates^ |> List.rev;
    Worker.post_message(updatesToSend);
    clearUpdates();
    print_endline("Posted!");

    /* print_endline ("Trying to marshal..."); */
    /* let updates = Marshal.to_string(_pendingUpdates^, []); */
    /* print_endline ("Marshalled value: " ++ updates); */
};

/* let init = app => { */

/*     let win = */ 
/*         App.createWindow(app, "Welcome to Revery", ~createOptions={ */
/*             ...Window.defaultCreateOptions, */
/*             maximized: true, */
/*         }); */


/*     let render = () => { */
/*         print_endline ("rendering"); */
/*         (renderFunction^)(); */
/*     }; */

/*     Window.maximize(win); */

/*     UI.start(win, render); */
/*     Window.setShouldRenderCallback(win, () => true); */
/* }; */

/* let startPlayground = () => { */
/*     App.start(init); */
/* }; */
