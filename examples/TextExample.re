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

let overflowStyles = (style, textWidth) =>
  Style.[
    color(Colors.white),
    width(textWidth),
    textOverflow(style),
    lineHeight(1.5),
    border(~color=Colors.blueViolet, ~width=1),
    backgroundColor(Colors.black),
  ];

module SampleText = {
  let%component make = () => {
    let%hook (fontSizeSliderVal, setFontSize) =
      Hooks.reducer(~initialState=20., (value, _) => value);
    let%hook (widthSliderVal, setWidth) =
      Hooks.reducer(~initialState=200., (value, _) => value);
    let%hook (hyphenate, setHyphenate) = Hooks.state(false);

    let textContent =
      "All work and no play makes Jack a dull boy. "
      ++ "The quick brown fox jumps over the lazy dog.";
    let maxFontSize = 40.;
    let maxWidth = 400.;
    let textFontSize = fontSizeSliderVal;
    let textWidth = int_of_float(widthSliderVal);
    let wrapping =
      if (hyphenate) {TextWrapping.WrapHyphenate} else {TextWrapping.Wrap};

    <View style=containerStyle>
      <View>
        <View style=Style.[height(40), marginBottom(8)]>
          <Text
            fontSize=textFontSize
            style={overflowStyles(`Ellipsis, textWidth)}
            text={textContent ++ " " ++ textContent}
          />
        </View>
        <View style=Style.[height(40), marginBottom(8)]>
          <Text
            fontSize=textFontSize
            style={overflowStyles(`UserDefined("£"), textWidth)}
            text={textContent ++ " " ++ textContent}
          />
        </View>
        <View style=Style.[height(80)]>
          <Text
            style=Style.[
              color(Colors.white),
              lineHeight(1.5),
              textWrap(wrapping),
              width(int_of_float(widthSliderVal)),
              border(~color=Colors.blueViolet, ~width=1),
              backgroundColor(Colors.black),
            ]
            fontSize=textFontSize
            text=textContent
          />
        </View>
      </View>
      <View>
        <View style=controlsStyle>
          <Text style=textStyle fontSize=16. text="Font size: " />
          <Slider
            onValueChanged={v => setFontSize(v)}
            value=fontSizeSliderVal
            maximumValue=maxFontSize
          />
          <Text
            style=textStyle
            fontSize=16.
            text={"Value: " ++ (fontSizeSliderVal |> string_of_float)}
          />
        </View>
        <View style=controlsStyle>
          <Text style=textStyle fontSize=16. text="Width: " />
          <Slider
            onValueChanged={w => setWidth(w)}
            value=widthSliderVal
            maximumValue=maxWidth
          />
          <Text
            style=textStyle
            fontSize=16.
            text={"Value: " ++ (widthSliderVal |> string_of_float)}
          />
        </View>
        <View style=controlsStyle>
          <Text style=textStyle fontSize=16. text="Hyphenate?" />
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
