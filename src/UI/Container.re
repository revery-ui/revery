open Revery_Core;

type state = {
  rendered: React.RenderedElement.t,
  previousElement: React.syntheticElement,
};

type t = {
  node: Reconciler.reveryNode,
  state: option(state),
};

let create: Reconciler.reveryNode => t = n => {node: n, state: None};

let update: (t, React.syntheticElement) => t =
  ({node, state}, element) => {
    let newRendered =
      switch (state) {
      | None =>
        let updates = React.RenderedElement.render(node, element);
        React.RenderedElement.executeHostViewUpdates(updates) |> ignore;
        updates |> React.RenderedElement.executePendingEffects;
      | Some(s) =>
        let nextElement =
          Performance.bench("RenderedElement.update", () =>
            React.RenderedElement.update(
              ~previousElement=s.previousElement,
              ~renderedElement=s.rendered,
              element,
            )
          );
        let nextElement =
          Performance.bench("RenderedElement.flushPendingUpdates", () =>
            React.RenderedElement.flushPendingUpdates(nextElement)
          );

        Performance.bench("RenderedElement.executeHostViewEffects", () =>
          React.RenderedElement.executeHostViewUpdates(nextElement) |> ignore
        );

        let ret =
          Performance.bench("RenderedElement.executePendingEffects", () =>
            React.RenderedElement.executePendingEffects(nextElement)
          );
        ret;
      };

    let ret: t = {
      node,
      state: Some({rendered: newRendered, previousElement: element}),
    };
    ret;
  };
