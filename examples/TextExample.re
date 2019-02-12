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

module SampleText = {
  let component = React.component("Example");

  let createElement = (~children as _, ()) =>
    component((slots) => {
      let (fontSizeSliderVal, setFontSize, _slots: React.Hooks.empty) = React.Hooks.state(20., slots);

      let textContent = "All work and no play makes Jack a dull boy";
      let maxFontSize = 40.;

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
              width(200),
              border(~color=Colors.blueViolet, ~width=1),
            ]
            text=textContent
          />
        </View>
        <View>
          <Text style=textStyle text="Font size: " />
          <Slider
            onValueChanged=setFontSize
            value=fontSizeSliderVal
            maximumValue=maxFontSize
          />
        </View>
      </View>;
    });
};

let render = () => <SampleText />;
