open Revery.Core;
open Revery.UI;
open Revery.UI.Components;

module RadioExample = {
  let component = React.component("RadioExample");
  let make = () => {
    component((_slots: React.Hooks.empty) => {
      <View
        style=Style.[
        width(500),
        height(500),
        justifyContent(`Center),
        alignItems(`Center),
        backgroundColor(Color.rgba(1., 1., 1., 0.1))
        ]>
        <Text 
          text="Radio Button"
          style=Style.[fontSize(20), fontFamily("Roboto-Regular.ttf"), marginBottom(20)]/>
        <RadioGroup>
          <RadioButton text="Gryffindor"/>
        </RadioGroup>
      </View>
    })
  }
  let createElement = (~children as _, ()) => React.element(make());
};

let render = () => <RadioExample />
