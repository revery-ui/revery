open Rejest;

/* open Revery_Math; */
open Revery_UI;

module UiReact = Revery_UI.Internal.UiReact;

/*
 * This module tests the integration of our `reason-reactify`'d reconciler - `UiReact`
 * with our Node tree.
 */

module TestRefComponent = (val component((render, ~latestRef,  ~children, ()) => {
    render(() => {
       let (refFromState, setRef) = useState(None); 

       latestRef := refFromState;


        let setRefInState = (r) => {
            setRef(Some(r));
        };

       <view ref={setRefInState} />;
    }, ~children)
}));

test("Reconciler", () => {
  test("reconcile adds a child", () => {
    let rootNode = (new viewNode)();

    let container = UiReact.createContainer(rootNode);
    UiReact.updateContainer(container, <view />);

    expect(List.length(rootNode#getChildren())).toBe(1);
  });

  test("ref: returns value of node", () => {
    let rootNode = (new viewNode)();

    let container = UiReact.createContainer(rootNode);

    /* Use a ref to track the latest value of the `ref={..}` callback */
    let referenceNode = ref(None);

    let refCallback = (r) => {
        referenceNode := Some(r);
    }

    UiReact.updateContainer(container, <view ref={refCallback}/>);
    rootNode#_flushEvents();

    /* And validate that we actually got the right one, based on the node ID! */
    switch (referenceNode^) {
    | Some(r) => {
       let actualChild = rootNode#firstChild();
       expect(actualChild#getInternalId()).toBe(r#getInternalId()); 
    }
    | None => expect(0).toBe(1);
    };
  });

  test("ref: validate ref gets passed back to component", () => {
    let rootNode = (new viewNode)();

    let container = UiReact.createContainer(rootNode);

    /*
     * Hold a `ref` that tracks the last refNode that comes from RENDER - 
     * this also exercises the interplay between the `ref` callback and `useState`
     */
    let latestRef: ref(option(viewNode)) = ref(None);

    UiReact.updateContainer(container, <TestRefComponent latestRef />);

    rootNode#_flushEvents();

    /*
     * After the update, we'd expect the following sequence:
     * - ref callback is called, trigger a setState in the component
     * - the setState triggers reconcile for that sub-tree
     * - that sub-tree is re-rendered, with a proper ref
     *
     * If that sequence is correct - we should have a value for `latestRef`,
     * because it's being set by the `render` function.
     */

    switch (latestRef^) {
    | Some(_) => {
        expect(1).toBe(1);
    }
    | None => expect(0).toBe(1);
    };
  });
});
