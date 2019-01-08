open Rejest;

/* open Revery_Math; */
open Revery_UI;

module UiReact = Revery_UI.Internal.UiReact;

/*
 * This module tests the integration of our `reason-reactify`'d reconciler - `UiReact`
 * with our Node tree.
 */

test("Reconciler", () => {
  test("reconcile adds a child", () => {
    let rootNode = (new viewNode)();

    let container = UiReact.createContainer(rootNode);

    /* print_endline ("START"); */

    UiReact.updateContainer(container, <view />);

    /* print_endline ("DONE"); */
    
    expect(List.length(rootNode#getChildren())).toBe(1);
  });

  test("ref: returns value of node", () => {
    let rootNode = (new viewNode)();

    let container = UiReact.createContainer(rootNode);

    /* print_endline ("START"); */

    let referenceNode = ref(None);

    let refCallback = (r) => {
        referenceNode := Some(r);
    }

    UiReact.updateContainer(container, <view ref={refCallback}/>);

    switch (referenceNode^) {
    | Some(r) => {
       let actualChild = rootNode#firstChild();
       expect(actualChild#getInternalId()).toBe(r#getInternalId()); 
    }
    | None => expect(0).toBe(1);
    };
  });
});
