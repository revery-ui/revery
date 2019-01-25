open Revery.Core;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

module AdjustableLogo = {
  let component = React.component("AdjustableLogo");

  let make = () =>
    component(slots => {
      let (rotationX, setRotationX, slots) = React.Hooks.state(0., slots);
      let (rotationY, setRotationY, slots) = React.Hooks.state(0., slots);
      let (rotationZ, setRotationZ, _slots: React.Hooks.empty) =
        React.Hooks.state(0., slots);

      let containerStyle =
        Style.[
          flexGrow(1),
          justifyContent(`Center),
          alignItems(`Center),
          flexDirection(`Column),
        ];

      let textStyle =
        Style.[
          color(Colors.white),
          width(100),
          fontFamily("Roboto-Regular.ttf"),
          fontSize(16),
          margin(14),
        ];

      let controlsStyle =
        Style.[
          margin(10),
          flexDirection(`Row),
          justifyContent(`Center),
          alignItems(`Center),
        ];

      let sliderContainerStyle =
        Style.[
          margin(10),
          borderBottom(~width=1, ~color=Colors.darkGray),
          flexDirection(`Row),
          justifyContent(`Center),
          alignItems(`Center),
        ];

      let verticalSliderContainerStyle =
        Style.[
          margin(10),
          borderRight(~width=1, ~color=Colors.darkGray),
          flexDirection(`Column),
          justifyContent(`Center),
          alignItems(`Center),
        ];

      let toDeg = r => 180. *. r /. pi;

      let toDegString = r => r |> toDeg |> floor |> string_of_float;

      let twoPi = 2. *. pi;

      <View style=containerStyle>
        <View>
          <Image
            src="outrun-logo.png"
            style=Style.[
              width(512),
              height(256),
              transform([
                Transform.RotateZ(Angle.from_radians(rotationZ)),
                Transform.RotateY(Angle.from_radians(rotationY)),
                Transform.RotateX(Angle.from_radians(rotationX)),
              ]),
            ]
          />
        </View>
        <View style=controlsStyle>
          <View style=verticalSliderContainerStyle>
            <Text style=textStyle text="Rotation Y: " />
            <Slider
              vertical=true
              onValueChanged=setRotationY
              maximumValue=twoPi
            />
            <Text
              style=textStyle
              text={"Value: " ++ toDegString(rotationY)}
            />
          </View>
          <View style=containerStyle>
            <View style=sliderContainerStyle>
              <Text style=textStyle text="Rotation X: " />
              <Slider
                onValueChanged=setRotationX
                value=twoPi
                maximumValue=twoPi
              />
              <Text
                style=textStyle
                text={"Value: " ++ toDegString(rotationX)}
              />
            </View>
            <View style=sliderContainerStyle>
              <Text style=textStyle text="Rotation Z: " />
              <Slider onValueChanged=setRotationZ maximumValue=twoPi />
              <Text
                style=textStyle
                text={"Value: " ++ toDegString(rotationZ)}
              />
            </View>
          </View>
        </View>
      </View>;
    });

  let createElement = (~children as _, ()) => React.element(make());
};

let render = () => <AdjustableLogo />;
