open Revery_UI;
open Revery_Core;

module RadioButton = {
  let component = React.component("RadioButton");
  let make = (~text, _children) => {
    component((_slots: React.Hooks.empty) => {
      <Clickable
        onClick=(() => print_endline("clicked"))
        style=Style.[justifyContent(`Center), flexDirection(`Row)]>
        <Text 
          text={||}
          style=Style.[fontSize(12), fontFamily("FontAwesome5FreeSolid.otf")]/>
        <Text 
          text
          style=Style.[
            color(Colors.white),
            fontSize(16),
            marginLeft(10),
            fontFamily("Roboto-Regular.ttf"),
          ]/>
      </Clickable>
    })
  };
  let createElement = (~children as _, ~text,()) => React.element(make(~text, ()));
};

let component = React.component("RadioGroup");

let make = children =>
component((_slots: React.Hooks.empty) => {
  <View>...children</View>
});

let createElement = (~children, ()) => React.element(make(children))

