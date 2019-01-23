open Revery_UI;
open Revery_Core;

let component = React.component("Checkbox");

let defaultStyle =
  Style.[
    width(50),
    height(50),
    border(~width=5, ~color=Colors.dodgerBlue),
  ];

let make = (~checked, ~style, ~onChange, ()) =>
  component(slots => {
    let (isChecked, checkBox, _slots: React.Hooks.empty) =
      React.Hooks.state(checked, slots);
    let bgColor = isChecked ? Colors.dodgerBlue : Colors.transparentWhite;
    let checkedContent = isChecked ? {||} : "";
    let stylesToUse = Style.merge(~source=defaultStyle, ~target=style);

    <Clickable
      onClick={() => {
        checkBox(!isChecked);
        onChange(!isChecked);
      }}>
      <View
        style=Style.(
          merge(
            ~source=[
              backgroundColor(bgColor),
              justifyContent(`Center),
              alignItems(`Center),
            ],
            ~target=stylesToUse,
          )
        )>
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

let createElement =
    (~children as _, ~checked=false, ~style=defaultStyle, ~onChange=noop, ()) =>
  React.element(make(~checked, ~onChange, ~style, ()));
