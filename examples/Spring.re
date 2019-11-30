open Revery;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

module SpringyLogo = {
  let%component make = (~stiffness, ~damping, ()) => {
    let%hook (targetPosition, setTargetPosition) = Hooks.state(1.0);

    let%hook imageWidth =
      Hooks.spring(
        targetPosition,
        Hooks.Spring.Options.create(
          ~damping,
          ~stiffness,
          ~initialValue=2.0,
          (),
        ),
      );

    let onMouseDown = _ => setTargetPosition(_ => 2.0);
    let onMouseUp = _ => setTargetPosition(_ => 1.0);

    let imageWidthI = imageWidth *. 256. |> int_of_float;

    <View onMouseDown onMouseUp>
      <Image
        src="outrun-logo.png"
        style=Style.[
          width(imageWidthI),
          height(128),
          cursor(MouseCursors.pointer),
        ]
      />
    </View>;
  };
};

module SliderControl = {
  module Styles = {
    let sliderContainerStyle =
      Style.[
        margin(10),
        borderBottom(~width=1, ~color=Colors.darkGray),
        flexDirection(`Row),
        justifyContent(`Center),
        alignItems(`Center),
      ];

    let textStyle =
      Style.[
        color(Colors.white),
        width(100),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(16),
        margin(14),
      ];
  };

  let float_rounded_string = v => v |> int_of_float |> string_of_int;

  let make =
      (~text, ~minimumValue, ~maximumValue, ~value, ~onValueChanged, ()) => {
    <View style=Styles.sliderContainerStyle>
      <Text style=Styles.textStyle text />
      <Slider
        onValueChanged={v => onValueChanged(v)}
        value
        minimumValue
        maximumValue
      />
      <Text
        style=Styles.textStyle
        text={"Value: " ++ float_rounded_string(value)}
      />
    </View>;
  };
};


module Example = {
  module Styles = {
    let containerStyle =
      Style.[
        flexGrow(1),
        justifyContent(`Center),
        alignItems(`Center),
        flexDirection(`Column),
      ];
  };
  let%component make = () => {
    let%hook (stiffness, setStiffness) = Hooks.state(160.);
    let%hook (damping, setDamping) = Hooks.state(10.);

    <Center>
      <Center> <SpringyLogo stiffness damping /> </Center>
      <View style=containerStyle>
        <SliderControl
          text="Damping: "
          minimumValue=1.
          maximumValue=100.
          value=damping
          onValueChanged={v => setDamping(_ => v)}
        />
        <SliderControl
          text="Stiffness: "
          minimumValue=1.
          maximumValue=500.
          value=stiffness
          onValueChanged={v => setStiffness(_ => v)}
        />
      </View>
    </Center>;
  };
};

let render = () => <Example />;
