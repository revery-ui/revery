open Revery;
open Revery.UI;
open Revery.UI.Components;

module Check = {
  type checkboxState = {
    first: bool,
    second: bool,
  };

  let getCheckboxText = checked => checked ? "Checked!" : "Not Checked!";

  let%component make = () => {
    let initialCheckboxState = {first: false, second: true};
    let%hook ({first, second}, setCheckboxState) =
      Hooks.state(initialCheckboxState);

    <View
      style=Style.[
        width(500),
        height(500),
        justifyContent(`Center),
        alignItems(`Center),
      ]>
      <Checkbox
        checked=first
        onChange={() => setCheckboxState(_ => {first: !first, second})}
        style=Style.[marginBottom(10)]
      />
      <Text
        text={getCheckboxText(first)}
        fontSize=20.
        style=Style.[marginBottom(10)]
      />
      <Checkbox
        checkedColor=Colors.green
        onChange={() => setCheckboxState(_ => {second: !second, first})}
        style=Style.[border(~width=2, ~color=Colors.green)]
        checked=second
      />
      <Text
        text={
          "Default state: " ++ getCheckboxText(initialCheckboxState.second)
        }
        fontSize=20.
        style=Style.[marginTop(10)]
      />
    </View>;
  };
};

let render = () => {
  <Check />;
};
