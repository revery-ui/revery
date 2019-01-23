open Revery_UI;
open Revery_Core;

let component = React.component("Checkbox");

let make = (~checked, ~onChange, ()) =>
  component(slots => {
    let (isChecked, checkBox, _slots: React.Hooks.empty) =
      React.Hooks.state(checked, slots);
    let bgColor = isChecked ? Colors.dodgerBlue : Colors.transparentWhite;
    let checkedContent = isChecked ? {||} : "";

    <Clickable
      onClick={() => {
        checkBox(!isChecked);
        onChange(!isChecked);
      }}>
      <View
        style=Style.[
          width(50),
          height(50),
          backgroundColor(bgColor),
          border(~width=5, ~color=Colors.dodgerBlue),
          justifyContent(`Center),
          alignItems(`Center),
        ]>
        <Text
          text=checkedContent
          style=Style.[
            color(isChecked ? Colors.white : Colors.black),
            fontSize(30),
            fontFamily("FontAwesome5FreeSolid.otf"),
          ]
        />
      </View>
    </Clickable>;
  });
let noop = _c => ();

let createElement = (~children as _, ~checked=false, ~onChange=noop, ()) =>
  React.element(make(~checked, ~onChange, ()));
