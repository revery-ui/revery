open Revery;
open Revery.UI;
open Revery.UI.Components;

module Styles = {
  open Style;

  let container = [];

  let toggle = [
    flexDirection(`Row),
    justifyContent(`Center),
    alignItems(`Center),
  ];

  let item = (~borderOpacity) => [
    border(~width=2, ~color=Color.rgba(0., 1., 1., borderOpacity)),
  ];

  let list = [];

  let text = [
    marginBottom(10),
    fontFamily("Roboto-Regular.ttf"),
    fontSize(16),
  ];
};

let toggle = (~checked, ~onChange, ()) =>
  <View style=Styles.toggle>
    <Checkbox checked onChange />
    <Text text="Toggle Item 3" style=Styles.text />
  </View>;

let%component item = (~label, ()) => {
  let%hook (borderOpacity, _, _) =
    Hooks.animation(Animation.(animate(Time.ms(500)) |> tween(1., 0.)));

  <View style={Styles.item(~borderOpacity)}>
    <Text text=label style=Styles.text />
  </View>;
};

let list = (~children, ()) => <View style=Styles.list> children </View>;

let items = [
  (React.Key.create(), "Item 1"),
  (React.Key.create(), "Item 2"),
  (React.Key.create(), "Item 3"),
  (React.Key.create(), "Item 4"),
];

let%component container = () => {
  let%hook (checked, setChecked) = Hooks.state(false);

  let items =
    checked ? items : List.filter(((_, label)) => label != "Item 3", items);

  <View style=Styles.container>
    <toggle checked onChange={() => setChecked(checked => !checked)} />
    <list>
      {items
       |> List.map(((_, label)) => <item key label />)
       |> React.listToElement}
    </list>
  </View>;
};

let render = () => <container />;
