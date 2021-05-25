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
  ];

let textStyle = Style.[marginBottom(20)];

module DropdownExample = {
  let items: DropdownInt.items = [
    {value: 1, label: "First option"},
    {value: 2, label: "Second option"},
    {value: 3, label: "Third option"},
    {value: 4, label: "Fourth option"},
    {value: 5, label: "A really, really, really long option"},
  ];

  let%component make = () => {
    let%hook (selectedItem, setSelectedItem) =
      Hooks.state(List.nth(items, 0));
    <View style=containerStyle>
      <Text
        style=textStyle
        fontSize=20.
        text={"Selected Item: " ++ selectedItem.label}
      />
      <DropdownInt items onItemSelected={item => setSelectedItem(_ => item)} />
    </View>;
  };
};

let render = () => <DropdownExample />;
