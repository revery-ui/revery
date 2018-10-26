# Revery 

Build __native__, _high-performance_, __cross-platform__ desktop apps with [reason](https://reasonml.github.io/)!

## Building & Installing

### Install [esy](https://esy.sh/)

`esy` is like `npm` for native code. If you don't have it already, install it by running:
```
npm install -g esy
```

### Building

- `esy install`
- `esy build`

### Running

After you build, the executables will be available in the `_build\install\default\bin` folder.

To run the example app, run `_build\install\default\bin\Bin.exe`.

### Tests

Tests can be run with:

- `esy b dune runtest`

## Motivation

Today, [Electron](https://electronjs.org/) is one of the most popular tools for building desktop apps - using an HTML, JS, CSS stack. However, it has a heavy footprint in terms of both RAM and CPU - __essentially packing an entire browser into the app.__ Even with that downsides, it has a lot of great aspects - it's the quickest way to build a cross-platform app & it provides a great development experience - as can be testified by its usage in popular apps like VSCode, Discord, and Slack.

Revery is kind of like super-fast, _native_ Electron - with a bundled React, Redux, and a fast build system - all ready to go!

Revery is built with [reasonml](https://reasonml.github.io), which is a javascript-like syntax on top of OCaml. This means that the language is accessible JS developers. Your apps are compiled to native code with the Reason / OCaml toolchain - with __instant startup__ and __performance comparable to native C code.__ Revery also features GPU-accelerated rendering. The compiler itself is fast, too!

Revery is an experiment - can we provide a great developer experience and help teams be productive, without making sacrifices on performance? Revery is built with [reasonml](https://reasonml.github.io), which is a javascript-like syntax on top of OCaml - meaning that Revery is accessible to JS developers. 

### Design Decisions

- __Consistent cross-platform behavior__

A major value prop of Electron is that you can build for all platforms at once. You have great confidence as a developer that your app will look and work the same across different platforms. Revery is the same - aside from platform-specific behavior, if your app looks or behaves differently on another platform, that's a bug! As a consequence, Revery is like [flutter](https://flutter.io) in that it __does not use native widgets__. This means more work for us, but also that we have more predictable functionality cross-platform!

> __NOTE:__ If you're looking for something that does leverage native widgets, check out [briskml](https://github.com/briskml/brisk)

- __High performance__

Performance should be at the forefront, and not a compromise - we need to develop and build benchmarks that help ensure top-notch performance and start-up time.

- __Type-safe, functional code__

We might have some dirty mutable objects for performance - but our high-level API should be purely functional. You should be able to follow the React model of modelling your UI as a _pure function_ of application state -> UI.

## Quickstart

TODO

## API Example

Here's a super simple Revery app, demonstrating the basic API surface:

```
open Revery;
open Revery.Core;
open Revery.UI;

/* The 'main' function for our app */
let init = app => {

  /* Create a window! */
  let w = App.createWindow(app, "test");

  /* Create a UI 'container' */
  let ui = UI.create(w);

  /* Set up some styles */
  let textHeaderStyle = Style.make(~backgroundColor=Colors.black, ~color=Colors.white, ~fontFamily="Roboto-Regular.ttf", ~fontSize=24, ());

  /* Set up our render function */
  Window.setRenderCallback(w, () => {

    /* This is where we render the UI - if you've used React or ReasonReact, it should look familiar */
    UI.render(ui,
        <view style=(Style.make(~position=LayoutTypes.Absolute, ~bottom=10, ~top=10, ~left=10, ~right=10, ~backgroundColor=Colors.blue, ()))>
            <view style=(Style.make(~position=LayoutTypes.Absolute, ~bottom=0, ~width=10, ~height=10, ~backgroundColor=Colors.red, ())) />
            <image src="logo.png" style=(Style.make(~width=128, ~height=64, ())) />
            <text style=(textHeaderStyle)>"Hello World!"</text>
            <view style=(Style.make(~width=25, ~height=25, ~backgroundColor=Colors.green, ())) />
        </view>);
  });
};

/* Let's get this party started! */
App.start(init);
```

### Custom Components

TODO

## Roadmap

It's early days for `revery` and we still have a lot of work ahead! 

Some tentative work we need to do, in no particular order:

- UI Infrastructure
    - [x] Styles
    - [ ] State management / Redux-like layer
    - [ ] Focus Management
    - [ ] Input handling
    - [ ] Animations
    - [ ] Gestures
    - [ ] Transforms
    - [ ] Compositing / Container
    - [ ] zIndex / layers
- UI Components
    - [x] View
    - [x] Image
    - [x] Text
    - [ ] TextInput
    - [ ] Button
    - [ ] Slider
    - [ ] Checkbox
    - [ ] ScrollView
- Platform support
    - [x] Windows
    - [x] OSX
    - [x] Linux
    - [ ] Web (JS + Wasm)
- Mobile support
    - [ ] Compilation to iOS
    - [ ] Compilation to Android
- Developer Experience
    - [ ] Hot reloading
    - [ ] 'Time travel' debugging across states
    - [ ] Integrated debugger
    - [ ] Integrated performance profiler
- Audio Support
    - [ ] Wav file playback
    - [ ] MP3 file playback
    - [ ] 3D / Spatial Audio
- Example apps
    - [ ] Quickstart / Hello World
    - [ ] Calculator
    - [ ] Todo List

## License

Revery is provided under the [MIT License](LICENSE).

## Contributing

We'd love your help, and welcome PRs and contributions.

Some ideas for getting started:
- Help us build example apps
- Help us implement missing features
- Help us log bugs

## Special Thanks

`revery` would not be possible without a bunch of cool tech:
- [reasonml](https://reasonml.github.io) made revery possible - thanks @jordwalke!
- [flex](https://github.com/jordwalke/flex) by @jordwalke
- [reason-reactify](https://github.com/bryphe/reason-reactify)
    - [Didact's DUI React tutorial](https://engineering.hexacta.com/didact-learning-how-react-works-by-building-it-from-scratch-51007984e5c5)
    - [ReactMini](https://github.com/reasonml/reason-react/tree/master/ReactMini)
- [reason-glfw](https://github.com/bryphe/reason-glfw)
    - [GLFW](https://www.glfw.org) - 
    - [stb-image](https://github.com/nothings/stb)
- [reason-fontkit](https://github.com/bryphe/reason-fontkit)
    - [freetype2](https://www.freetype.org)
    - [harfbuzz](https://www.freedesktop.org/wiki/Software/HarfBuzz)
- [reason-gl-matrix](https://github.com/bryphe/reason-gl-matrix)
    - [gl-matrix](http://glmatrix.net)
    - [glm](https://glm.g-truc.net/0.9.9/index.html)

`revery` was inspired by some __awesome projects:__
- [react-native](https://facebook.github.io/react-native/)
- [briskml](https://github.com/briskml/brisk)
- [elm](https://elm-lang.org/)
