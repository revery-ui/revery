open Revery;
open Revery.UI;
open Revery.UI.Components;

let containerStyle =
  Style.[
    position(`Absolute),
    top(0),
    bottom(0),
    left(0),
    right(0),
    alignItems(`Center),
    justifyContent(`Center),
    flexDirection(`Column),
  ];

let controlsStyle =
  Style.[
    margin(10),
    flexDirection(`Row),
    justifyContent(`Center),
    alignItems(`Center),
  ];

let textStyle =
  Style.[
    color(Colors.white),
    width(100),
    margin(14),
    textWrap(TextWrapping.NoWrap),
  ];

module Example = {
  type inputFields = {
    first: string,
    second: string,
    third: string,
    isPassword: bool,
  };

  let%component make = () => {
    let%hook ({first, isPassword, _}, setValue) =
      Hooks.state({first: "", second: "", third: "", isPassword: false});

    <View style=containerStyle>
      <View
        style=Style.[
          flexDirection(`Row),
          alignItems(`Center),
          justifyContent(`Center),
        ]>
        <Input
          placeholder="Insert text here"
          onChange={(value, _) =>
            setValue(state => {...state, first: value})
          }
          value=first
        />
        <Button
          height=50
          width=100
          fontSize=15.
          title="Reset"
          onClick={() => setValue(state => {...state, first: ""})}
        />
        <Button
          height=50
          width=100
          fontSize=15.
          title="Set value"
          onClick={() => setValue(state => {...state, first: "New value"})}
        />
      </View>
      <Padding padding=20>
        <View
          style=Style.[
            flexDirection(`Row),
            alignItems(`Center),
            justifyContent(`Center),
          ]>
          <Input
            placeholder="Insert text here"
            onChange={(value, _) =>
              setValue(state => {...state, first: value})
            }
            value=first
            isPassword
          />
          <View style=controlsStyle>
            <Text fontSize=16. style=textStyle text="Obscure Input" />
            <Checkbox
              checkedColor=Colors.green
              onChange={() =>
                setValue(state => {...state, isPassword: !state.isPassword})
              }
              style=Style.[border(~width=2, ~color=Colors.green)]
              checked=isPassword
            />
          </View>
        </View>
      </Padding>
      <Padding padding=20>
        <BoxShadow
          boxShadow={Style.BoxShadow.make(
            ~xOffset=-5.,
            ~yOffset=2.,
            ~color=Colors.black,
            ~blurRadius=20.,
            ~spreadRadius=0.,
            (),
          )}>
          <Input
            placeholder="custom input"
            placeholderColor=Colors.plum
            cursorColor=Colors.white
            autofocus=true
            onFocus={() => print_endline("Input example focused")}
            onBlur={() => print_endline("Input example blurred")}
            onChange={(value, _) =>
              setValue(state => {...state, second: value})
            }
            onKeyDown={_ => print_endline("key event")}
            style=Style.[
              backgroundColor(Colors.paleVioletRed),
              color(Colors.white),
              height(50),
            ]
          />
        </BoxShadow>
      </Padding>
    </View>;
  };
};

let render = () => <Example />;
