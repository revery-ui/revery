open Revery_UI;
open Revery_Core;
open Revery_UI_Primitives;

let component = React.component("Checkbox");

let defaultStyle =
  Style.[
    width(50),
    height(50),
    border(~width=5, ~color=Colors.dodgerBlue),
  ];

let noop = _c => ();

let createElement =
    (
      ~children as _,
      ~checked=false,
      ~checkedColor=Colors.dodgerBlue,
      ~style=defaultStyle,
      ~onChange=noop,
      (),
    ) =>
  component(slots => {
    let stylesToUse = Style.merge(~source=defaultStyle, ~target=style);
    let bgColor = checked ? checkedColor : Colors.transparentWhite;
    let checkedContent = checked ? {||} : "";

    (
      slots,
      <Clickable onClick=onChange>
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
              color(checked ? Colors.white : Colors.black),
              fontSize(30),
              fontFamily("FontAwesome5FreeSolid.otf"),
            ]
          />
        </View>
      </Clickable>,
    );
  });
