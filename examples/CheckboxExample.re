open Revery.Core;
open Revery.UI;
open Revery.UI.Components;

module Check = {
  let component = React.component("Check");
  let createElement = (~children as _, ()) => 
    component(slots => {
      let (text, setText, slots) =
        React.Hooks.state("Not Checked!", slots);
      (slots, <View
        style=Style.[
          width(500),
          height(500),
          justifyContent(`Center),
          alignItems(`Center),
        ]>
        <Checkbox
          onChange={checked => {
            let text = checked ? "Checked!" : "Not Checked!";
            setText(text);
          }}
          style=Style.[marginBottom(10)]
        />
        <Text
          text
          style=Style.[
            marginBottom(10),
            fontFamily("Roboto-Regular.ttf"),
            fontSize(20),
          ]
        />
        <Checkbox
          checkedColor=Colors.green
          style=Style.[border(~width=2, ~color=Colors.green)]
          checked=true
        />
        <Text
          text="Default state: Checked"
          style=Style.[
            marginTop(10),
            fontFamily("Roboto-Regular.ttf"),
            fontSize(20),
          ]
        />
      </View>);
    });
};

let render = () => {
  <Check />;
};
