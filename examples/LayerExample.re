open Revery.UI;

module Layers = {
  let make = () => {
    <Layer condition=RenderCondition.initial style=Style.[flexGrow(1)]>
      <View
        style=Style.[
          flexDirection(`Column),
          flexGrow(1),
          justifyContent(`Center),
          alignItems(`Center),
          backgroundColor(Revery.Colors.red),
        ]>
        <Text text="Hello from inside a layer!" />
      </View>
    </Layer>;
  };
};

let render = () => <Layers />;
