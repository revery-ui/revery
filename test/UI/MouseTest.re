open Rejest;

open Revery_UI;

let createNodeWithStyle = (style) => {
  let node = (new node)();
  node#setStyle(style);
  Layout.layout(node, 1);
  node;
}

test("Mouse", () => {
  test("dispatch", () => {
    test("triggers onMouseDown event for node", () => {

      let cursor = Mouse.Cursor.make();

      let count = ref(0);
      let f = (_evt) => count := count^ + 1;
      let node = createNodeWithStyle(Style.make(~width=100, ~height=100, ()));
      node#setEvents(NodeEvents.make(~onMouseDown=f, ()));  

      Mouse.dispatch(cursor, InternalMouseMove({mouseX: 50., mouseY: 50. }), node);
      Mouse.dispatch(cursor, InternalMouseDown({button: BUTTON_LEFT}), node);

      expect(count^).toBe(1);
    });
    test("does not trigger onMouseUp event for node if outside node", () => {
      let cursor = Mouse.Cursor.make();

      let count = ref(0);
      let f = (_evt) => count := count^ + 1;
      let node = createNodeWithStyle(Style.make(~width=100, ~height=100, ()));
      node#setEvents(NodeEvents.make(~onMouseDown=f, ()));  

      Mouse.dispatch(cursor, InternalMouseMove({mouseX: 150., mouseY: 150. }), node);
      Mouse.dispatch(cursor, InternalMouseUp({button: BUTTON_LEFT}), node);

      expect(count^).toBe(0);
    });
  });
});
