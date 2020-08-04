open Revery;
open Revery.UI;
open Revery.UI.Components;

open Revery.Native;

module Example = {
  let menu = Menu.create(~title="Zach's Menu");

  let%component make = (~window, ()) => {
    let%hook (lastClicked, setLastClicked) = Hooks.state(None);

    let%hook () =
      Hooks.effect(
        OnMount,
        () => {
          let menuItems =
            Array.init(
              10,
              i => {
                let item =
                  Menu.Item.create(~title="Item #" ++ string_of_int(i));
                let () =
                  item
                  |> Menu.Item.setOnClick(item =>
                       print_endline(Utilities.MacOS.nsObjectToString(item))
                     );
                item;
              },
            );
          let () =
            menuItems |> Array.iter(item => {Menu.addItem(menu, item)});
          None;
        },
      );

    Gc.full_major();

    <View>
      <Center>
        <Button
          title="Display menu!"
          onClick={() =>
            Menu.displayAt(
              ~window=Window.getSdlWindow(window),
              ~x=200,
              ~y=200,
              menu,
            )
          }
        />
        {switch (lastClicked) {
         | Some(i) => <Text text={"Clicked on Item #" ++ string_of_int(i)} />
         | None => React.empty
         }}
      </Center>
    </View>;
  };
};

let render = window => <Example window />;
