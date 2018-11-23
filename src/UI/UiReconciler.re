/* Reconciler.re
 * 
 * This implements a reconciler for our UI primitives
 */

open RenderPass;

type primitives = 
    | View(Style.t)
    | Text(Style.t, string)
    | Image(Style.t, string);


type node = Node.node(renderPass);

let createInstance = (prim) => {
    let node = switch (prim) {
    | View(_style) => 
        let view = new ViewNode.viewNode("test")
        view#setStyle(_style);
        view;
    | Image(style, src) => 
        let img = new ImageNode.imageNode("test", src);
        img#setStyle(style);
        img
    | Text(style, text) =>
        let text = new TextNode.textNode("test", text);
        text#setStyle(style);
        (text :> node);
    };

    node
};

let updateInstance = (n: node, _oldPrim: primitives, newPrim: primitives) => {
    switch (newPrim) {
    | Text(style, text) => 
        /* TODO: Is there a way to downcast properly here, from Node -> TextNode ? */
        let tn: TextNode.textNode = Obj.magic(n);
        tn#setStyle(style);
        tn#setText(text);
    | Image(style, _src) => 
        let imageNode: ImageNode.imageNode = Obj.magic(n);
        imageNode#setStyle(style);
    | _ => ()
    }
};

let appendChild = (parent: node, child: node) => {
    parent#addChild(child);
};

let removeChild = (parent: node, child: node) => {
    parent#removeChild(child);
};

let replaceChild = (parent: node, newChild: node, oldChild: node) => {
    removeChild(parent, oldChild);
    appendChild(parent, newChild);
    ()
};
