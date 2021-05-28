open Revery;
open Revery.UI;
open Revery.UI.Components;

open Revery.Native;

module Internal = {
  let nonce = ref(0);
};

module View = {
  let make = () => {
    let onClickCreateMenu = () => {
      incr(Internal.nonce);
      let currentNonce = Internal.nonce^;
      let startTime = Time.now();
      let menuBar = Menu.getMenuBarHandle();
      Menu.clear(menuBar);

      let menu1 = Menu.create("Test 1");
      Menu.addSubmenu(~parent=menuBar, ~child=menu1);

      let menuCallback = (str, ~fromKeyPress, ()) => {
        print_endline(
          Printf.sprintf(
            "%s clicked: %d (from key press: %b)",
            str,
            currentNonce,
            fromKeyPress,
          ),
        );
      };

      let item11 =
        Menu.Item.create(
          ~title="Item 1.1",
          ~onClick=menuCallback("Item 1.1"),
          ~keyEquivalent=Menu.KeyEquivalent.ofString("a"),
          (),
        );
      Menu.addItem(menu1, item11);

      let menu2 = Menu.create("Test 2");
      Menu.addSubmenu(~parent=menuBar, ~child=menu2);

      let item12 =
        Menu.Item.create(
          ~title="Item 1.2",
          ~onClick=menuCallback("Item 1.2"),
          ~keyEquivalent=
            Menu.KeyEquivalent.ofString("b")
            |> (ke => Menu.KeyEquivalent.enableShift(ke, true)),
          (),
        );
      let item13 =
        Menu.Item.create(
          ~title="Item 1.3",
          ~onClick=menuCallback("Item 1.3"),
          ~keyEquivalent=
            Menu.KeyEquivalent.ofString(" ")
            |> (ke => Menu.KeyEquivalent.enableCtrl(ke, true)),
          (),
        );
      Menu.addItem(menu2, item12);
      Menu.addItem(menu2, item13);

      let menu3 = Menu.create("Test 3");
      Menu.addSubmenu(~parent=menuBar, ~child=menu3);

      let subMenu31 = Menu.create("Submenu 1");
      Menu.addSubmenu(~parent=menu3, ~child=subMenu31);

      let item311 =
        Menu.Item.create(
          ~title="Item 3.1.1",
          ~onClick=menuCallback("Item 1.3"),
          ~keyEquivalent=
            Menu.KeyEquivalent.ofString("c")
            |> (ke => Menu.KeyEquivalent.enableAlt(ke, true)),
          (),
        );

      let separator = Menu.Item.createSeparator();

      let item312 =
        Menu.Item.create(
          ~title="Item 3.1.2",
          ~onClick=menuCallback("Item 1.4"),
          (),
        );

      Menu.Item.setEnabled(item312, false);

      let item313 =
        Menu.Item.create(
          ~title="Item 3.1.3",
          ~onClick=menuCallback("Item 1.5"),
          (),
        );

      Menu.Item.setVisible(item313, false); // Shouldn't show up

      Menu.addItem(subMenu31, item311);
      Menu.addItem(subMenu31, separator);
      Menu.addItem(subMenu31, item312);
      Menu.addItem(subMenu31, item313);

      let endTime = Time.now();
      let delta = Time.(endTime - startTime) |> Time.toFloatSeconds;
      print_endline("Render time: " ++ string_of_float(delta));
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
