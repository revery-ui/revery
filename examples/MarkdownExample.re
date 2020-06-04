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
    <markdown
      markdown="

# reason-sdl2

#### Based on [reason-glfw](https://github.com/revery-ui/reason-glfw). Reason / OCaml bindings for SDL2

## Building

`esy` is used for building, so if you don't have `esy`, install it:

```
npm install -g esy
```

```OCaml
let x = 4 in
let y = x - 10 in
let z = y * x;;
```

Then just run `esy` in the root of the project to install and build.

## Running

Run `esy @example install` initially to install, then `esy @example run` to build and run the example.

## License

This project is licensed under the MIT License - see [LICENSE](LICENSE) for more details.

## Design

TBD

## Acknowledgements

- The test texture is provided by [LearnOpenGL](https://learnopengl.com) - an excellent resource for OpenGL!
- This is built on several great libraries, like [SDL2](https://libsdl.org), [glad](https://github.com/Dav1dde/glad), [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h)
- The test image is from @Arahnoid's [UVChecker-map](https://github.com/Arahnoid/UVChecker-map) repo.

    "
    />
  </View>;

let render = () => <example />;
