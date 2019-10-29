open Revery;
open Revery.UI;
open Revery.UI.Components;

module CanQuit = {
  type checkboxState = {canQuit: bool};

  let%component make = () => {
    let initialCheckboxState = {canQuit: true};
    let%hook ({canQuit}, setCheckboxState) =
      Hooks.state(initialCheckboxState);

    <View
      style=Style.[
        width(500),
        height(500),
        justifyContent(`Center),
        alignItems(`Center),
      ]>
      <Checkbox
        checkedColor=Colors.green
        onChange={() => {
          let win = getActiveWindow();
          switch (win) {
          | Some(win) => Window.setCanQuitCallback(win, () => !canQuit)
          | None => ()
          };
          setCheckboxState(({canQuit}) => {canQuit: !canQuit});
        }}
        style=Style.[border(~width=2, ~color=Colors.green)]
        checked=canQuit
      />
      <Text
        text={"Can quit: " ++ (canQuit ? "Yes" : "No")}
        style=Style.[
          marginTop(10),
          fontFamily("Roboto-Regular.ttf"),
          fontSize(20),
        ]
      />
    </View>;
  };
};

let render = () => {
  <CanQuit />;
};
