module Reconciler = UiReconciler.Reconciler;

let onStale = Reconciler.onStale;

module React = Brisk_reconciler.Make(Reconciler);

module Container = {
  type state = {
    rendered: React.RenderedElement.t,
    previousElement: React.syntheticElement,
  };

  type t = {
    node: UiReconciler.reveryNode,
    state: option(state),
  };

  let createContainer: UiReconciler.reveryNode => t =
    n => {node: n, state: None};

  let updateContainer: (t, React.syntheticElement) => t =
    ({node, state}, element) => {
      let newRendered =
        switch (state) {
        | None =>
          let updates = React.RenderedElement.render(node, element);
          React.RenderedElement.executeHostViewUpdates(updates) |> ignore;
          updates |> React.RenderedElement.executePendingEffects;
        | Some(s) =>
          let nextElement =
            React.RenderedElement.update(
              ~previousElement=s.previousElement,
              ~renderedElement=s.rendered,
              element,
            )
            |> React.RenderedElement.flushPendingUpdates;
          React.RenderedElement.executeHostViewUpdates(nextElement) |> ignore;
          nextElement |> React.RenderedElement.executePendingEffects;
        };

      let ret: t = {
        node,
        state: Some({rendered: newRendered, previousElement: element}),
      };
      ret;
    };
};

include React;
