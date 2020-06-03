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
  let linkActive = [color(Colors.blue)];
  let linkInactive = [color(Colors.lightBlue)];
};

let markdown =
  Markdown.make(
    ~paragraphStyle=Styles.whiteText,
    ~h1Style=Styles.whiteText,
    ~h2Style=Styles.whiteText,
    ~h3Style=Styles.whiteText,
    ~h4Style=Styles.whiteText,
    ~h5Style=Styles.whiteText,
    ~h6Style=Styles.whiteText,
    ~activeLinkStyle=Styles.linkActive,
    ~inactiveLinkStyle=Styles.linkInactive,
  );

let example = () =>
  <View style=Styles.outer>
    <Text text="Font variations: " />
    <markdown markdown="
Normal **Bold** _Italicized_
      " />
    <Text text="Links: " />
    <markdown
      markdown="
[I'm an inline-style link](https://www.google.com)

[I'm an inline-style link with **BOLD**](https://www.google.com)

[I'm an inline-style link with __Italics__](https://www.google.com)
      "
    />
    <Text text="Blockquotes: " />
    <markdown
      markdown="
> Cras dignissim leo vel mi iaculis accumsan. In hac habitasse platea dictumst. Aliquam erat volutpat. Nunc eu diam risus. Nulla elementum feugiat urna, in porttitor dui cursus vel. Integer congue lobortis nulla hendrerit viverra. Cras egestas velit dolor. Morbi accumsan risus nulla, id ultricies turpis aliquet sed. Suspendisse vulputate turpis a ex ultricies ullamcorper.
"
    />
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
