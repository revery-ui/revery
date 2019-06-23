open Revery;
open Revery.UI;
open Revery.UI.Components;

let addMenu = () => {
  switch (Revery_Native.Menu.getApplicationMenu()) {
  | Some(menu) =>
    let _ =
      Revery_Native.Menu.(
        appendMenus(
          menu,
          [
            <String label="New Item" callback={() => print_endline("hey")} />,
          ],
        )
      );
    ();
  | None => ()
  };
};

let render = _w => {
  <View
    style=Style.[
      position(`Absolute),
      justifyContent(`Center),
      alignItems(`Center),
      bottom(0),
      top(0),
      left(0),
      right(0),
    ]>
    <Button title="Add item to application menu" onClick=addMenu />
  </View>;
};
