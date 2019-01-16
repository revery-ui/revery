/* UiReconciler.re
 *
 * This implements a reconciler for our UI primitives
 */

open RenderPass;

type reveryNode = Node.node(renderPass);

module Reconciler = {
    type hostElement = reveryNode;     
    type node = reveryNode;

    let insertNode = (~parent: node, ~child: node, ~position as _) => {
        parent#addChild(child); 
        parent;
    };

    let deleteNode = (~parent: node, ~child: node) => {
        parent#removeChild(child);   
        parent;
    };

    let moveNode = (~parent, ~child as _, ~from as _, ~to_ as _) => {
        parent;
    };

    let markAsStale = () => prerr_endline ("MarkAsStale");

    let beginChanges = () => ();
    let commitChanges = () => ();
}
