open Rejest;

open Revery_UI;
open Revery_UI_Primitives;

/*
 * This module tests the integration of the Brisk reconciler - `React` -
 * with our Node tree.
 */

module TestRefComponent = {
  let component = React.component("TestRefComponent");

  let createElement = (~children as _, ~latestRef, ()) =>
    component(hooks => {
      let (refFromState, setRef, hooks) = React.Hooks.state(None, hooks);

      latestRef := refFromState;

      let setRefInState = r => {
        prerr_endline("SET REF IN STATE");
        setRef(Some(r));
      };

      (hooks, <View ref=setRefInState />);
    });
};

test("Reconciler", () => {
  test("reconcile adds a child", () => {
    let rootNode = (new viewNode)();

    let container = Container.create(rootNode);
    Container.update(container, <View />) |> ignore;

    expect(List.length(rootNode#getChildren())).toBe(1);
  });

  test("ref: returns value of node", () => {
    let rootNode = (new viewNode)();

    let container = Container.create(rootNode);

    /* Use a ref to track the latest value of the `ref={..}` callback */
    let referenceNode = ref(None);

    let refCallback = r => referenceNode := Some(r);

    Container.update(container, <View ref=refCallback />) |> ignore;
    rootNode#flushCallbacks();

    /* And validate that we actually got the right one, based on the node ID! */
    switch (referenceNode^) {
    | Some(r) =>
      let actualChild = rootNode#firstChild();
      expect(actualChild#getInternalId()).toBe(r#getInternalId());
    | None => expect(0).toBe(1)
    };
  });

  test("ref: validate ref gets passed back to component", () => {
    let rootNode = (new viewNode)();

    let container = Container.create(rootNode);

    /*
     * Hold a `ref` that tracks the last refNode that comes from RENDER -
     * this also exercises the interplay between the `ref` callback and `useState`
     */
    let latestRef: ref(option(viewNode)) = ref(None);

    /* First update - this will end up 'mounting' the node and dispatching the 'ref' callback */
    /* However - the state won't be updated - it will just be queued up */
    let update1 = Container.update(container, <TestRefComponent latestRef />);

    rootNode#flushCallbacks();

    /* We need to update again to pick up the state update */
    Container.update(update1, <TestRefComponent latestRef />) |> ignore;

    switch (latestRef^) {
    | Some(_) => expect(1).toBe(1)
    | None => expect(0).toBe(1)
    };
  });

  test("layout: onDimensionsChanged gets dispatched", () => {
    let rootNode = (new viewNode)();
    let container = Container.create(rootNode);
    rootNode#setStyle(
      Style.create(~style=Style.[width(100), height(200)], ()),
    );

    let callCount = ref(0);
    let lastWidth = ref(0);
    let lastHeight = ref(0);

    let style =
      Style.[position(`Absolute), top(0), left(0), right(0), bottom(0)];

    let onDimensionsChanged =
        ({width, height}: NodeEvents.DimensionsChangedEventParams.t) => {
      lastWidth := width;
      lastHeight := height;
      callCount := callCount^ + 1;
    };

    let update1 =
      Container.update(container, <View onDimensionsChanged style />);

    Layout.layout(rootNode);
    rootNode#recalculate();
    rootNode#flushCallbacks();

    expect(callCount^).toBe(1);
    expect(lastWidth^).toBe(100);
    expect(lastHeight^).toBe(200);

    /* Shouldn't dispatch if width/height hasn't change! */
    let update2 =
      Container.update(update1, <View onDimensionsChanged style />);

    Layout.layout(rootNode);
    rootNode#recalculate();
    rootNode#flushCallbacks();

    expect(callCount^).toBe(1);

    /* Should update if the size was somehow changed.. */
    rootNode#setStyle(
      Style.create(~style=Style.[width(300), height(400)], ()),
    );
    Container.update(update2, <View onDimensionsChanged style />) |> ignore;

    Layout.layout(rootNode);
    rootNode#recalculate();
    rootNode#flushCallbacks();

    expect(callCount^).toBe(2);
    expect(lastWidth^).toBe(300);
    expect(lastHeight^).toBe(400);
  });
});
