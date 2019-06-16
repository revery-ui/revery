open Revery;
open Revery.UI;
open Revery.UI.Components;

module RadioExample = {
  let component = React.component("RadioExample");
  let make = () =>
    component(hooks => {
      let (radioVal, setRadioVal, hooks) =
        Hooks.state("Select a button!", hooks);
      (
        hooks,
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
            onChange={txt => setRadioVal("Radio Button Value: " ++ txt)}
            defaultSelected=0
            buttons=[
              {text: "Button 1", value: "1"},
              {text: "Button 2", value: "2"},
              {text: "Button 3", value: "3"},
              {text: "Button 4", value: "4"},
            ]
          />
          <Text
            text=radioVal
            style=Style.[
              fontSize(20),
              fontFamily("Roboto-Regular.ttf"),
              marginTop(20),
            ]
          />
        </View>,
      );
    });
  let createElement = (~children as _, ()) => make();
};

let render = () => <RadioExample />;
