/* UiReconciler.re
 *
 * This implements a reconciler for our UI primitives
 */

open RenderPass;
open Revery_Core;

type reveryNode = Node.node(renderPass);

module Reconciler = {
  type hostElement = reveryNode;
  type node = reveryNode;

  let onStale: Event.t(unit) = Event.create();

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

  let markAsStale = () => {
    Event.dispatch(onStale, ());
  };

  let beginChanges = () => ();
  let commitChanges = () => ();
};
