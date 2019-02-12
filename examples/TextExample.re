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

let slidersViewStyle = Style.[height(200)];

let textStyle =
  Style.[
    color(Colors.white),
    width(100),
    fontFamily("Roboto-Regular.ttf"),
    fontSize(16),
    margin(14),
    textWrap(TextWrapping.NoWrap),
  ];

let controlsStyle =
  Style.[
    margin(10),
    flexDirection(`Row),
    justifyContent(`Center),
    alignItems(`Center),
  ];

module SampleText = {
  let component = React.component("Example");

  let createElement = (~children as _, ()) =>
    component(slots => {
      let (fontSizeSliderVal, setFontSize, slots) =
        React.Hooks.state(20., slots);
      let (widthSliderVal, setWidth, _slots: React.Hooks.empty) =
        React.Hooks.state(200., slots);

      let textContent = "All work and no play makes Jack a dull boy";
      let maxFontSize = 40.;
      let maxWidth = 400.;

      <View style=containerStyle>
        <View style=slidersViewStyle>
          <Text
            logging=true
            style=Style.[
              color(Colors.white),
              fontFamily("Roboto-Regular.ttf"),
              fontSize(int_of_float(fontSizeSliderVal)),
              lineHeight(1.5),
              textWrap(TextWrapping.WhitespaceWrap),
              width(int_of_float(widthSliderVal)),
              border(~color=Colors.blueViolet, ~width=1),
            ]
            text=textContent
          />
        </View>
        <View style=controlsStyle>
          <Text style=textStyle text="Font size: " />
          <Slider
            onValueChanged=setFontSize
            value=fontSizeSliderVal
            maximumValue=maxFontSize
          />
          <Text
            style=textStyle
            text={
              "Value: " ++ (fontSizeSliderVal |> int_of_float |> string_of_int)
            }
          />
        </View>
        <View style=controlsStyle>
          <Text style=textStyle text="Width: " />
          <Slider
            onValueChanged=setWidth
            value=widthSliderVal
            maximumValue=maxWidth
          />
          <Text
            style=textStyle
            text={
              "Value: " ++ (widthSliderVal |> int_of_float |> string_of_int)
            }
          />
        </View>
      </View>;
    });
};

let render = () => <SampleText />;
