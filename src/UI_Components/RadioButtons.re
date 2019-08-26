open Revery_UI;
open Revery_Core;
open Revery_UI_Primitives;

type button('a) = {
  text: string,
  value: 'a,
};
let defaultStyle =
  Style.[
    width(100),
    color(Colors.white),
    fontSize(16),
    marginLeft(10),
    fontFamily("Roboto-Regular.ttf"),
  ];

let component = a => React.component("RadioButtons", a);

let make =
    (
      ~defaultSelected,
      ~buttons: list(button('a)),
      ~iconSize,
      ~style,
      ~onChange,
      _children,
    ) =>
  component(slots => {
    let defaultVal = List.nth(buttons, defaultSelected).value;
    let (checkedVal, setCheckedVal, slots) =
      React.Hooks.state(defaultVal, slots);

    let icon = v => v == checkedVal ? {||} : {||};

    (
      slots,
      <View style=Style.[justifyContent(`Center), alignItems(`Center)]>
        ...{
             buttons
             |> List.map(button =>
                  <Clickable
                    onClick={() => {
                      setCheckedVal(button.value);
                      onChange(button.value);
                    }}
                    style=Style.[
                      justifyContent(`Center),
                      flexDirection(`Row),
                      alignItems(`Center),
                      height(30),
                    ]>
                    <Text
                      text={icon(button.value)}
                      style=Style.[
                        fontSize(iconSize),
                        fontFamily("FontAwesome5FreeSolid.otf"),
                      ]
                    />
                    <Text text={button.text} style />
                  </Clickable>
                )
           }
      </View>,
    );
  });

let createElement =
    (
      ~children as _,
      ~defaultSelected,
      ~buttons,
      ~iconSize=12,
      ~style=defaultStyle,
      ~onChange=_ => (),
      (),
    ) =>
  make(~defaultSelected, ~buttons, ~iconSize, ~style, ~onChange, ());
