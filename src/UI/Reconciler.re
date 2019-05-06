/* Reconciler.re
 *
 * This implements a reconciler for our UI primitives.
 * This is the 'back-end' for our React API - we hand this
 * over to the Brisk_reconciler.Make functor, and get our
 * nice React API that works against Revery's node tree.
 */

open Revery_Core;

type reveryNode = Node.node;

type hostElement = reveryNode;
type node = reveryNode;

let onStale: Event.t(unit) = Event.create();

let insertNode = (~parent: node, ~child: node, ~position as _) => {
  parent#addChild(child);
  parent;
};

let deleteNode = (~parent: node, ~child: node, ~position as _) => {
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
