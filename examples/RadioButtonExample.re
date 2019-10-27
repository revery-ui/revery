open Revery;
open Revery.UI;
open Revery.UI.Components;

module RadioExample = {
  let%component make = () => {
    let%hook (radioVal, setRadioVal) = Hooks.state("Select a button!");

    <View
      style=Style.[
        width(500),
        height(500),
        justifyContent(`Center),
        alignItems(`Center),
        backgroundColor(Color.rgba(1., 1., 1., 0.1)),
      ]>
      <Text
        text="Radio Button"
        style=Style.[
          fontSize(20),
          fontFamily("Roboto-Regular.ttf"),
          marginBottom(20),
        ]
      />
      <RadioButtons
        onChange={txt =>
          setRadioVal("Radio Button Value: " ++ string_of_int(txt))
        }
        defaultSelected=0
        buttons=[
          {text: "Button 1", value: 1},
          {text: "Button 2", value: 2},
          {text: "Button 3", value: 3},
          {text: "Button 4", value: 4},
        ] // it is of type list(RadioButtons.button(int))
      />
      <Text
        text=radioVal
        style=Style.[
          fontSize(20),
          fontFamily("Roboto-Regular.ttf"),
          marginTop(20),
        ]
      />
    </View>;
  };
};

let render = () => <RadioExample />;
