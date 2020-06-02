open Revery;
open Revery.UI;

module Styles = {
  open Style;

  let family =
    FontFamily.asset(
      ~variant=
        (fontWeight, italicized, monospaced) =>
          switch (fontWeight, italicized) {
          | (Bold, false) => "Roboto-Bold.ttf"
          | (Bold, true) => "Roboto-BoldItalic.ttf"
          | (Normal, true) => "Roboto-RegularItalic.ttf"
          | _ => "Roboto-Regular.ttf"
          },
      "Roboto-Regular.ttf",
    );

  let textStyle = [fontFamily(family), color(Colors.white), fontSize(14.)];

  let boldStyle = [
    fontFamily(family),
    fontWeight(Font.Weight.Bold),
    color(Colors.white),
    fontSize(14.),
  ];

  let italicStyle = [
    fontFamily(family),
    color(Colors.white),
    italicized(true),
    fontSize(14.),
  ];

  let boldItalicStyle = [
    fontFamily(family),
    color(Colors.white),
    fontWeight(Font.Weight.Bold),
    italicized(true),
    fontSize(14.),
  ];

  let outer = [
    position(`Absolute),
    top(0),
    bottom(0),
    left(0),
    right(0),
    justifyContent(`Center),
    alignItems(`Center),
  ];
};

let fontFamilies = () =>
  <View style=Styles.outer>
    <Text text="Lorem ipsum su dolorit" style=Styles.textStyle />
    <Text text="Lorem ipsum su dolorit" style=Styles.boldStyle />
    <Text text="Lorem ipsum su dolorit" style=Styles.italicStyle />
    <Text text="Lorem ipsum su dolorit" style=Styles.boldItalicStyle />
  </View>;

let render = () => <fontFamilies />;
