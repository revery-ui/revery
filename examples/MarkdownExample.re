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
    margin(6),
    justifyContent(`FlexStart),
    alignItems(`FlexStart),
  ];

  let whiteText = [color(Colors.white)];
};

let markdown =
  Markdown.make(
    ~paragraphStyle=Styles.whiteText,
    ~h1Style=Styles.whiteText,
    ~h2Style=Styles.whiteText,
    ~h3Style=Styles.whiteText,
    ~h4Style=Styles.whiteText,
    ~baseFontSize=16.0,
  );

let example = () =>
  <View style=Styles.outer>
    <Text text="Font variations: " />
    <markdown markdown="
Normal **Bold** _Italicized_
      " />
    <Text text="Headings: " />
    <markdown
      markdown="
# H1
## H2
### H3
#### H4
##### H5
###### H6
      "
    />
  </View>;

let render = () => <example />;
