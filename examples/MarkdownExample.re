open Revery;
open Revery.UI;
open Revery.UI.Components;

module Styles = {
  open Style;

  let outer = [
    position(`Absolute),
    top(0),
    bottom(0),
    left(0),
    right(0),
    justifyContent(`FlexStart),
    alignItems(`FlexStart),
  ];

  let paragraph = [
    color(Colors.white),
    fontFamily("Asap"),
    fontSize(14.),
  ];

  let bold = [
    color(Colors.white),
    fontFamily("Roboto-Bold.ttf"),
    fontSize(14.),
  ];

  let italic = [
    color(Colors.white),
    fontFamily("Roboto-RegularItalic.ttf"),
    fontSize(14.),
  ];

  let h1 = [
    color(Colors.white),
    fontFamily("Roboto-Bold.ttf"),
    fontSize(36.),
  ];

  let h2 = [
    color(Colors.white),
    fontFamily("Roboto-Bold.ttf"),
    fontSize(30.),
  ];

  let h3 = [
    color(Colors.white),
    fontFamily("Roboto-Bold.ttf"),
    fontSize(24.),
  ];

  let h4 = [
    color(Colors.white),
    fontFamily("Roboto-Bold.ttf"),
    fontSize(18.),
  ];
};

let example = () =>
  <View style=Styles.outer>
    <Markdown
      paragraphStyle=Styles.paragraph
      boldStyle=Styles.bold
      italicStyle=Styles.italic
      h1Style=Styles.h1
      h2Style=Styles.h2
      h3Style=Styles.h3
      h4Style=Styles.h4
      markdown="
# Markdown in Revery?!?!
What is this magic?

## Different header levels :O
**Bolded text too!**

### Weee we're getting smaller!
__Italics have joined the chat__
      "
    />
  </View>;

let render = () => <example />;
