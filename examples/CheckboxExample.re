open Revery;
open Revery.UI;
open Revery.UI.Components;

module Check = {
  type checkboxState = {
    first: bool,
    second: bool,
  };

  let getCheckboxText = checked => checked ? "Checked!" : "Not Checked!";

  let component = React.component("Check");

  let createElement = (~children as _, ()) =>
    component(hooks => {
      let initialCheckboxValues = {first: false, second: true};
      let ({first, second}, setCheckboxState, hooks) =
        React.Hooks.state(initialCheckboxValues, hooks);

      (
        hooks,
        <View
          style=Style.[
            width(500),
            height(500),
            justifyContent(`Center),
            alignItems(`Center),
          ]>
          <Checkbox
            checked=first
            onChange={() => setCheckboxState({first: !first, second})}
            style=Style.[marginBottom(10)]
          />
          <Text
            text={getCheckboxText(first)}
            style=Style.[
              marginBottom(10),
              fontFamily("Roboto-Regular.ttf"),
              fontSize(20),
            ]
          />
          <Checkbox
            checkedColor=Colors.green
            onChange={() => setCheckboxState({second: !second, first})}
            style=Style.[border(~width=2, ~color=Colors.green)]
            checked=second
          />
          <Text
            text={
              "Default state: "
              ++ getCheckboxText(initialCheckboxValues.second)
            }
            style=Style.[
              marginTop(10),
              fontFamily("Roboto-Regular.ttf"),
              fontSize(20),
            ]
          />
        </View>,
      );
    });
};

let render = () => {
  <Check />;
};
