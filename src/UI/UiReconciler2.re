
open RenderPass;

type reveryNode = Node.node(renderPass);

module Renderer = {
    type hostElement = reveryNode;     
    type node = reveryNode;

    let insertNode = (~parent: node, ~child: node, ~position as _) => {
        parent#addChild(child); 
    };

    let deleteNode = (~parent: node, ~child: node) => {
        parent#removeChild(child);   
    };

    let moveNode = (~parent, ~child as _, ~from as _, ~to_ as _) => {
        parent;
    };

    let markAsStale = () => ();

    let beginChanges = () => ();
    let commitChanges = () => ();
}
