open Revery.UI;

module View = {
  let noop = () => ();
  let%component make = () => {
    let%hook (isColumn, setIsColumn) = Hooks.state(true);

    let containerStyle =
      Style.[
        position(`Absolute),
        justifyContent(`Center),
        alignItems(`Center),
        flexDirection(isColumn ? `Column : `Row),
        bottom(0),
        top(0),
        left(0),
        right(0),
      ];

    <View style=containerStyle>
      <NativeButton
        title="Toggle Direction"
        onClick={() => setIsColumn(ic => !ic)}
      />
      <NativeButton
        title="Print to STDOUT"
        onClick={() => print_endline("You clicked a button!")}
      />
      <NativeButton title="Custom Width" style=Style.[width(200)] />
      <NativeButton title="Custom Height" style=Style.[height(50)] />
      <NativeButton
        title="Custom Both"
        style=Style.[width(200), height(50)]
      />
    </View>;
  };
};

let render = () => <View />;
