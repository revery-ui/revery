open Rejest;

open Revery_UI;

/*
 * This module tests the integration of the Brisk reconciler - `React` -
 * with our Node tree.
 */

module TestRefComponent = {
  let component = React.component("TestRefComponent");

  let make = (~latestRef, ()) =>
    component(slots => {
      let (refFromState, setRef, _slots: React.Hooks.empty) =
        React.Hooks.state(None, slots);

      latestRef := refFromState;

      let setRefInState = r => {
        prerr_endline("SET REF IN STATE");
        setRef(Some(r));
      };

      <View ref=setRefInState />;
    });

  let createElement = (~children as _, ~latestRef, ()) =>
    React.element(make(~latestRef, ()));
};

test("Reconciler", () => {
  test("reconcile adds a child", () => {
    let rootNode = (new viewNode)();

    let container = React.Container.create(rootNode);
    React.Container.update(container, <View />) |> ignore;

    expect(List.length(rootNode#getChildren())).toBe(1);
  });

  test("ref: returns value of node", () => {
    let rootNode = (new viewNode)();

    let container = React.Container.create(rootNode);

    /* Use a ref to track the latest value of the `ref={..}` callback */
    let referenceNode = ref(None);

    let refCallback = r => {
      referenceNode := Some(r);
    };

    React.Container.update(container, <View ref=refCallback />) |> ignore;
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

    let container = React.Container.create(rootNode);

    /*
     * Hold a `ref` that tracks the last refNode that comes from RENDER -
     * this also exercises the interplay between the `ref` callback and `useState`
     */
    let latestRef: ref(option(viewNode)) = ref(None);

    /* First update - this will end up 'mounting' the node and dispatching the 'ref' callback */
    /* However - the state won't be updated - it will just be queued up */
    let update1 =
      React.Container.update(container, <TestRefComponent latestRef />);

    rootNode#flushCallbacks();

    /* We need to update again to pick up the state update */
    React.Container.update(update1, <TestRefComponent latestRef />) |> ignore;

    switch (latestRef^) {
    | Some(_) => expect(1).toBe(1)
    | None => expect(0).toBe(1)
    };
  });
});
