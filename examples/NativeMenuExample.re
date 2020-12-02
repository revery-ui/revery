// open Revery;
open Revery.UI;
open Revery.UI.Components;

open Revery.Native;

module View = {
  let make = () => {
    // let%hook (lastRenderTime, setLastRenderTime) = Hooks.state(None);
    // let%hook (lastClick, setLastClick) = Hooks.state(None);

    let onClickCreateMenu = () => {
      let menuBar = Menu.getMenuBarHandle();
      Menu.clear(menuBar);

      let menu1 = Menu.create("Test 1");
      Menu.addSubmenu(~parent=menuBar, ~child=menu1);

      let item11 = Menu.Item.create(~title="Item 1.1", ~onClick=() => ());
      Menu.addItem(menu1, item11);

      let menu2 = Menu.create("Test 2");
      Menu.addSubmenu(~parent=menuBar, ~child=menu2);

      let item12 = Menu.Item.create(~title="Item 1.2", ~onClick=() => ());
      Menu.addItem(menu2, item12);

      let menu3 = Menu.create("Test 3");
      Menu.addSubmenu(~parent=menuBar, ~child=menu3);

      let subMenu31 = Menu.create("Submenu 1");
      Menu.addSubmenu(~parent=menu3, ~child=subMenu31);

      let item311 = Menu.Item.create(~title="Item 3.1.1", ~onClick=() => ());

      Menu.addItem(subMenu31, item311);
    };

    let containerStyle =
      Style.[
        position(`Absolute),
        justifyContent(`Center),
        alignItems(`Center),
        bottom(0),
        top(0),
        left(0),
        right(0),
      ];

    <View style=containerStyle>
      <Button title="Create Menu" onClick=onClickCreateMenu />
    </View>;
  };
};

let render = () => <View />;
