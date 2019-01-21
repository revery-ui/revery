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
        Style.make(
          ~flexGrow=1,
          ~justifyContent=LayoutTypes.JustifyCenter,
          ~alignItems=LayoutTypes.AlignCenter,
          ~flexDirection=LayoutTypes.Column,
          (),
        );

      let textStyle =
        Style.make(
          ~color=Colors.white,
          ~width=100,
          ~fontFamily="Roboto-Regular.ttf",
          ~fontSize=16,
          ~margin=14,
          (),
        );
      let controlsStyle =
        Style.make(
          ~margin=10,
          ~flexDirection=LayoutTypes.Row,
          ~justifyContent=LayoutTypes.JustifyCenter,
          ~alignItems=LayoutTypes.AlignCenter,
          (),
        );

      let sliderContainerStyle =
        Style.make(
          ~margin=10,
          ~borderBottom=
            Style.Border.make(~width=1, ~color=Colors.darkGray, ()),
          ~flexDirection=LayoutTypes.Row,
          ~justifyContent=LayoutTypes.JustifyCenter,
          ~alignItems=LayoutTypes.AlignCenter,
          (),
        );

      let verticalSliderContainerStyle =
        Style.make(
          ~margin=10,
          ~borderRight=
            Style.Border.make(~width=1, ~color=Colors.darkGray, ()),
          ~flexDirection=LayoutTypes.Column,
          ~justifyContent=LayoutTypes.JustifyCenter,
          ~alignItems=LayoutTypes.AlignCenter,
          (),
        );

      let toDeg = r => 180. *. r /. pi;

      let toDegString = r => r |> toDeg |> floor |> string_of_float;

      let twoPi = 2. *. pi;

      <View style=containerStyle>
        <View>
          <Image
            src="outrun-logo.png"
            style={Style.make(
              ~width=512,
              ~height=256,
              ~transform=[
                RotateZ(Angle.from_radians(rotationZ)),
                RotateY(Angle.from_radians(rotationY)),
                RotateX(Angle.from_radians(rotationX)),
              ],
              (),
            )}
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
              <Slider onValueChanged=setRotationX maximumValue=twoPi />
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
