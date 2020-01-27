open Revery;
open Revery.UI;
open Revery.UI.Components;

let containerStyle =
  Style.[
    position(`Absolute),
    top(0),
    bottom(0),
    left(0),
    right(0),
    alignItems(`Center),
    justifyContent(`SpaceAround),
    flexDirection(`Column),
  ];

let slidersViewStyle = Style.[height(300)];

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

let overflowStyles = (style, textFontSize, textWidth) =>
  Style.[
    color(Colors.white),
    fontFamily("Roboto-Regular.ttf"),
    fontSize(textFontSize),
    width(textWidth),
    textOverflow(style),
    lineHeight(1.5),
    border(~color=Colors.blueViolet, ~width=1),
    backgroundColor(Colors.black),
  ];

module SampleText = {
  let%component make = () => {
    let%hook (fontSizeSliderVal, setFontSize) = Hooks.state(20.);
    let%hook (widthSliderVal, setWidth) = Hooks.state(200.);
    let%hook (hyphenate, setHyphenate) = Hooks.state(false);

    let textContent =
      "All work and no play makes Jack a dull boy. "
      ++ "The quick brown fox jumps over the lazy dog.";
    let maxFontSize = 40.;
    let maxWidth = 400.;
    let textFontSize = int_of_float(fontSizeSliderVal);
    let textWidth = int_of_float(widthSliderVal);
    let wrapping =
      if (hyphenate) {TextWrapping.WrapHyphenate} else {TextWrapping.Wrap};

    <View style=containerStyle>
      <View>
        <View style=Style.[height(40), marginBottom(8)]>
          <Text
            style={overflowStyles(`Ellipsis, textFontSize, textWidth)}
            text={textContent ++ " " ++ textContent}
          />
        </View>
        <View style=Style.[height(40), marginBottom(8)]>
          <Text
            style={overflowStyles(
              `UserDefined("£"),
              textFontSize,
              textWidth,
            )}
            text={textContent ++ " " ++ textContent}
          />
        </View>
        <View style=Style.[height(80)]>
          <Text
            style=Style.[
              color(Colors.white),
              fontFamily("Roboto-Regular.ttf"),
              fontSize(textFontSize),
              lineHeight(1.5),
              textWrap(wrapping),
              width(int_of_float(widthSliderVal)),
              border(~color=Colors.blueViolet, ~width=1),
              backgroundColor(Colors.black),
            ]
            text=textContent
          />
        </View>
      </View>
      <View>
        <View style=controlsStyle>
          <Text style=textStyle text="Font size: " />
          <Slider
            onValueChanged={v => setFontSize(_ => v)}
            value=fontSizeSliderVal
            maximumValue=maxFontSize
          />
          <Text
            style=textStyle
            text={"Value: " ++ (fontSizeSliderVal |> string_of_float)}
          />
        </View>
        <View style=controlsStyle>
          <Text style=textStyle text="Width: " />
          <Slider
            onValueChanged={w => setWidth(_ => w)}
            value=widthSliderVal
            maximumValue=maxWidth
          />
          <Text
            style=textStyle
            text={"Value: " ++ (widthSliderVal |> string_of_float)}
          />
        </View>
        <View style=controlsStyle>
          <Text style=textStyle text="Hyphenate?" />
          <Checkbox
            checkedColor=Colors.green
            onChange={() => setHyphenate(h => !h)}
            style=Style.[border(~width=2, ~color=Colors.green)]
            checked=hyphenate
          />
        </View>
      </View>
    </View>;
  };
};

let render = () => <SampleText />;
