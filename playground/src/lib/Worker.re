open Revery;
open Revery.UI;

open Js_of_ocaml;

open Types;

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
    Types.showAll(updatesToSend);
    Worker.post_message(updatesToSend);
    print_endline("Posted!" ++ string_of_int(List.length(updatesToSend)));
    clearUpdates();

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
