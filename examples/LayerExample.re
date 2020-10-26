open Revery.UI;

module Layers = {
  let make = () => {
    <Layer
      condition=Layer.Condition.always
      style=Style.[flexGrow(1)]
      backgroundColor=Revery.Colors.red>
      <View
        style=Style.[
          flexDirection(`Column),
          flexGrow(1),
          justifyContent(`Center),
          alignItems(`Center),
        ]>
        <Text text="Hello from inside a layer!" />
      </View>
    </Layer>;
  };
};

let render = () => <Layers />;
