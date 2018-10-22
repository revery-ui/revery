/* Reconciler.re
 * 
 * This implements a reconciler for our UI primitives
 */

open Revery_Core;

type primitives = 
    | View(Style.t)
    | Text
    | Image;


type node = Node.node;

let createInstance = (prim) => {
    let node = switch (prim) {
    | View(_style) => 
        let view = new ViewNode.viewNode("test")
        view#setStyle(_style);
        view;
    | _ => 
        let n = new Node.node("test");
        n#setStyle(Style.make(~width=50, ~height=50, ~backgroundColor=(Color.rgb(0.0, 1.0, 1.0)), ()));
        n;
    };


    node
};

let appendChild = (parent: node, child: node) => {
    parent#addChild(child);
};

let removeChild = (parent: node, child: node) => {
    parent#removeChild(child);
};

let updateInstance = () => {
    print_endline ("TODO: updateElement");
};

let replaceChild = (parent: node, newChild: node, oldChild: node) => {
    removeChild(parent, oldChild);
    appendChild(parent, newChild);
    ()
};
