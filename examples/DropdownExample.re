open Revery.UI;
/* open Revery.Core; */
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

/*    TODO: Allow user style overwrite for Dropdown component
 */

module Example = {
  let component = React.component("Example");

  let items: Dropdown.items = [
    {value: "1", label: "First option"},
    {value: "2", label: "Second option"},
    {value: "3", label: "Third option"},
    {value: "4", label: "Fourth option"},
  ];

  let make = () =>
    component((_slots: React.Hooks.empty) =>
      <View style=containerStyle> <Dropdown items /> </View>
    );

  let createElement = (~children as _, ()) => React.element(make());
};

let render = () => <Example />;
