open Rejest;

/* open Revery_Math; */
open Revery_UI;

module UiReact = Revery_UI.Internal.UiReact;

/*
 * This module tests the integration of our `reason-reactify`'d reconciler - `UiReact`
 * with our Node tree.
 */

test("Reconciler", () => {
  test("no children initially", () => {
    let viewNode = (new viewNode)();

    /* let container = UiReact.createContainer(viewNode); */

    /* print_endline ("START"); */

    /* UiReact.updateContainer(container, <view />); */

    /* print_endline ("DONE"); */
    
    expect(List.length(viewNode#getChildren())).toBe(1);
  });
});
