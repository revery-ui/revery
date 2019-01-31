open Revery.UI;
open Revery.UI.Components;
open Revery.Core;

open Revery.Math;

module Controls = {
  type inputFields = {
    first: string,
    second: string,
  };

  let component = React.component("Controls");
  let make = window =>
    component(slots => {
      let (count, setCount, slots) = React.Hooks.state(0, slots);

      let ({first, second}: inputFields, setValue, slots) =
        React.Hooks.state({first: "", second: ""}, slots);

      let (text, setText, slots) = React.Hooks.state("Not Checked!", slots);

      let (rotationX, setRotationX, slots) = React.Hooks.state(0., slots);
      let (rotationY, setRotationY, slots) = React.Hooks.state(0., slots);
      let (rotationZ, setRotationZ, _slots: React.Hooks.empty) =
        React.Hooks.state(0., slots);

      let increment = () => setCount(count + 1);

      let containerStyle =
        Style.[justifyContent(`FlexStart), alignItems(`FlexStart)];

      let topContainer =
        Style.[
          justifyContent(`Center),
          alignItems(`Center),
          flexDirection(`Row),
        ];

      let countContainer =
        Style.[
          width(200),
          height(200),
          alignItems(`Center),
          justifyContent(`Center),
        ];

      let inputContainer =
        Style.[
          width(200),
          height(200),
          alignItems(`Center),
          justifyContent(`Center),
        ];

      let checkboxContainer =
        Style.[
          width(200),
          height(200),
          alignItems(`Center),
          justifyContent(`Center),
        ];

      let countStyle =
        Style.[
          fontSize(30),
          margin(15),
          color(Colors.white),
          fontFamily("Roboto-Regular.ttf"),
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

      let countStr = string_of_int(count);
      <View style=containerStyle>
        <View style=topContainer>
          <View style=countContainer>
            <Text style=countStyle text=countStr />
            <Button
              width=150
              height=50
              fontSize=20
              title="click me!"
              onClick=increment
            />
            <Button
              width=150
              height=50
              fontSize=20
              disabled=true
              title="(disabled)"
              onClick=increment
            />
          </View>
          <View style=inputContainer>
            <Input
              window
              style=Style.[fontSize(20)]
              placeholder="Insert text here"
              onChange={(~value) => setValue({first: value, second})}
            />
            <Input
              window
              placeholder="custom input"
              placeholderColor=Colors.plum
              cursorColor=Colors.white
              onChange={(~value) => setValue({first, second: value})}
              style=Style.[
                backgroundColor(Colors.paleVioletRed),
                color(Colors.white),
                fontSize(20),
                boxShadow(
                  ~xOffset=-5.,
                  ~yOffset=2.,
                  ~color=Colors.black,
                  ~blurRadius=20.,
                  ~spreadRadius=0.,
                ),
              ]
            />
          </View>
          <View style=checkboxContainer>
            <Checkbox
              onChange={checked => {
                let text = checked ? "Checked!" : "Not Checked!";
                setText(text);
              }}
              style=Style.[marginBottom(10)]
            />
            <Text
              text
              style=Style.[
                marginBottom(10),
                fontFamily("Roboto-Regular.ttf"),
                fontSize(20),
              ]
            />
            <Checkbox
              checkedColor=Colors.green
              style=Style.[border(~width=2, ~color=Colors.green)]
              checked=true
            />
            <Text
              text="Default state: Checked"
              style=Style.[
                marginTop(10),
                fontFamily("Roboto-Regular.ttf"),
                fontSize(20),
              ]
            />
          </View>
        </View>
        <View>
          <Image
            src="outrun-logo.png"
            style=Style.[
              width(200),
              height(100),
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

  let createElement = (~window, ~children as _, ()) =>
    React.element(make(window));
};

let render = window => <Controls window />;