open Revery.UI;
open Revery.Core;
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
    flexDirection(`Column),
  ];

let outerBox =
  Style.[width(128), height(128), backgroundColor(Colors.black)];

let innerBox =
  Style.[
    width(450),
    height(450),
    backgroundColor(Color.rgba(0., 1., 0., 0.5)),
  ];

module Sample = {
  let component = React.component("Sample");

  let createElement = (~children as _, ()) =>
    component((slots) =>
      (slots, <View style=containerStyle>
        <ScrollView style=outerBox>
          <Image
            src="outrun-logo.png"
            style=Style.[width(512), height(256)]
          />
          <Image
            src="outrun-logo.png"
            style=Style.[width(512), height(256)]
          />
          <Image
            src="outrun-logo.png"
            style=Style.[width(512), height(256)]
          />
        </ScrollView>
      </View>)
    );
};

let render = () => <Sample />;
