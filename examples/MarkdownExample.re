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
    <markdown
      markdown="
Normal **Bold** _Italicized_ `public static void main(abc)`
      "
    />
    <Text text="Links: " />
    <markdown
      markdown="
[I'm an inline-style link](https://www.google.com)

[I'm an inline-style link with **BOLD**](https://www.google.com)

[I'm an inline-style link with __Italics__](https://www.google.com)

This is [id] reference-style link.

[id]: https://google.com/
      "
    />
    <Text text="Blockquotes: " />
    <markdown markdown="
> Level 1
>> Level 2
>>> Level 3
>>>> Level 4
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
    <Text text="Lists: " />
    <markdown
      markdown="
- Unordered
- List
    - Multiple
    - Levels

1. Ordered
2. List
    1. Multiple
    2. Levels

- Unordered
- List
    1. Ordered
    2. List
      "
    />
    <Text text="Thematic breaks: " />
    <markdown markdown="
Section A

---

Section B
      " />
  </View>;

let render = () => <example />;
