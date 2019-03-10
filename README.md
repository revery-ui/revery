<p align="center">
	<a href="https://www.outrunlabs.com/revery" title="Revery">
		<img src="./assets/logo.png" alt="Logo">
	</a>
</p>

<p align="center">
  <span>Build <b>native</b>, <i>high-performance</i>, <b>cross-platform</b> desktop tops with <a href="https://reasonml.github.io">reason!</a></span>
</p>

<p align="center">
  <a href="https://dev.azure.com/revery-ui/revery/_build/latest?definitionId=2?branchName=master">
    <img src="https://dev.azure.com/revery-ui/revery/_apis/build/status/revery-ui.revery?branchName=master" alt="Build Status"/>
  </a>
  <a href="https://badge.fury.io/js/revery">
    <img src="https://badge.fury.io/js/revery.svg" alt="npm version"/>
  </a>
  <a href="https://discord.gg/4pxY5Cp">
    <img src="https://img.shields.io/discord/526111832478449695.svg" alt="Join the chat on discord!"/>
  </a>
</p>

---

<p align="center">
	<a href="https://www.outrunlabs.com/revery/playground" title="Playground">
		<img src="./assets/screenshot.png" alt="Slider components">
	</a>
</p>

:construction: __NOTE:__ Revery is a work-in-progress and in active development! :construction:

To get a taste of Revery, check out our JavaScript + WebGL build on the [playground](https://outrunlabs.com/revery/playground). For the best experience, though, you'll want to try a native build :point_down:

## Building & Installing

### Install [esy](https://esy.sh/)

`esy` is like `npm` for native code. If you don't have it already, install it by running:
```
npm install -g esy
```

> __NOTE:__ Revery requires `esy` at version 0.5.6+

### Building

- `esy install`
- `esy build`

#### For `macOS` users

If your build takes too much time then you can pre-install some libraries:

- `brew install cmake`
- `brew install libpng ragel`

#### For `Linux` users

Install the following packages with your package manager of choice:

* `cmake`
* `ragel`

##### For `Ubuntu` you may need these additional packages
* `libpng-dev`
* `libbz2-dev`
* `m4`
* `xorg-dev`
* `libglu1-mesa-dev`
* `libharfbuzz-dev`
* `libgtk-3-dev`

> __NOTE:__ `reason-fontkit` (a dependency of `revery`) requires `harfbuzz` 1.7.7+. This means `revery` requires Ubuntu 18.10+ (or you can install `libharfbuzz-dev@1.8.8` by [adding 'cosmic' packages to your sources](https://makandracards.com/makandra/47558-how-to-install-packages-from-newer-ubuntu-releases), but proceed with caution since you can break other packages this way)

> __NOTE:__ If `ezy build` produces an error about `libpangoft2` then try deleting the `./_esy` and `~/.esy` directories and run run `ezy install` and `esy build` again.

##### For `Fedora` you may need these additional packages
* `libpng-devel`
* `bzip2-devel`
* `m4`
* `xorg-x11-server-devel`
* `mesa-libGLU-devel`
* `harfbuzz-devel`

> __NOTE:__ `reason-fontkit` (a dependency of `revery`) requires `harfbuzz` 1.7.7+. This means `revery` requires Fedora 29+

#### For `Windows` native

No additional dependencies needed. 

> __NOTE:__ `esy` requires building from an Administrator prompt (either `cmd.exe` or `Powershell`).

#### For `Windows` Subsystem for Linux (`WSL`)

Make sure to check the specific requirements for your Linux distribution, above.

> __NOTE:__ Hardware acceleration is not enabled by default in WSL (instead, WSL will fall-back to a _software_ renderer). This is problematic for performance - for that reason, we recommend building and running Revery natively on Windows instead of with WSL. For more info see: (https://github.com/Microsoft/WSL/issues/637 and a [potential workaround](https://superuser.com/questions/1238925/use-accelerated-graphics-on-windows-10-linux-subsystem-with-xming))

### Running

After building, you can run the example app by running:

- `esy x Examples`

Check out our [examples](https://github.com/revery-ui/revery/tree/master/examples) to see how they work!

### Tests

Tests can be run with:

- `esy b dune runtest`

## Motivation

Today, [Electron](https://electronjs.org/) is one of the most popular tools for building desktop apps - using an HTML, JS, CSS stack. However, it has a heavy footprint in terms of both RAM and CPU - __essentially packing an entire browser into the app.__ Even with that tradeoff, it has a lot of great aspects - it's the quickest way to build a cross-platform app & it provides a great development experience - as can be testified by its usage in popular apps like VSCode, Discord, and Slack.

Revery is kind of like super-fast, _native_ Electron - with bundled React-like/Redux-like libraries and a fast build system - all ready to go!

Revery is built with [reasonml](https://reasonml.github.io), which is a javascript-like syntax on top of [OCaml](https://ocaml.org) This means that the language is accessible to JS developers.

Your apps are compiled to native code with the Reason / OCaml toolchain - with __instant startup__ and __performance comparable to native C code.__ Revery also features GPU-accelerated rendering. The compiler itself is fast, too!

Revery is an experiment - can we provide a great developer experience and help teams be productive, without making sacrifices on performance?
### Design Decisions

- __Consistent cross-platform behavior__

A major value prop of Electron is that you can build for all platforms at once. You have great confidence as a developer that your app will look and work the same across different platforms. Revery is the same - aside from platform-specific behavior, if your app looks or behaves differently on another platform, that's a bug! As a consequence, Revery is like [flutter](https://flutter.io) in that it __does not use native widgets__. This means more work for us, but also that we have more predictable functionality cross-platform!

> __NOTE:__ If you're looking for something that does leverage native widgets, check out [briskml](https://github.com/briskml/brisk). Another alternative is the [cuite](https://github.com/let-def/cuite) OCaml binding for [Qt](https://github.com/let-def/cuite).

- __High performance__

Performance should be at the forefront, and not a compromise - we need to develop and build benchmarks that help ensure top-notch performance and start-up time.

- __Type-safe, functional code__

We might have some dirty mutable objects for performance - but our high-level API should be purely functional. You should be able to follow the React model of modelling your UI as a _pure function_ of application state -> UI.

## Quickstart

Check out [revery-quick-start](https://github.com/bryphe/revery-quick-start) to get up and running with your own Revery app!

## API Example

Here's a super simple Revery app, demonstrating the basic API surface:

```reason
/**
 * The 'main' function for our app.
 */
let init = app => {
  /* Create a window! */
  let win = App.createWindow(app, "test");

  /* Set up some styles */
  let textHeaderStyle =
    Style.[
      backgroundColor(Colors.black),
      color(Colors.white),
      fontFamily("Roboto-Regular.ttf"),
      fontSize(24),
    ];

  /* Set up render function */
  let render = () => {
    <view
      style={Style.[
        position(`Absolute),
        bottom(10),
        top(10),
        left(10),
        right(10),
        backgroundColor(Colors.blue),
      ]}>
      <view
        style={Style.[
          position(`Absolute),
          bottom(0),
          width(10),
          height(10),
          backgroundColor(Colors.red),
        ]}
      />
      <image src="logo.png" style={Style.make(~width=128, ~height=64, ())} />
      <text style=textHeaderStyle> "Hello World!" </text>
      <view
        style={Style.[
          width(25),
          height(25),
          backgroundColor(Colors.green),
        ]}
      />
    </view>;
  };

  /* Start the UI */
  UI.start(win, render);
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
    - [x] State management / Redux-like layer
    - [x] Focus Management
    - [x] Input handling
    - [x] Animations
    - [ ] Gestures
    - [x] Transforms
    - [ ] Compositing / Container
    - [ ] zIndex / layers
- UI Components
    - [x] View
    - [x] Image
    - [x] Text
    - [x] Input
    - [x] Button
    - [x] Slider
    - [x] Checkbox
    - [x] ScrollView
- Platform support
    - [x] Windows
    - [x] OSX
    - [x] Linux
    - [x] Web (JS + Wasm)
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
- Example apps
    - [x] [Quickstart](https://github.com/bryphe/revery-quick-start) / Hello World
    - [x] Calculator
    - [ ] Todo List

## License

Revery is provided under the [MIT License](LICENSE).

## Contributing

We'd love your help, and welcome PRs and contributions. Join us on [Discord](https://discord.gg/UvQ2cFn)!

Some ideas for getting started:
- Help us build example apps,
- Help us implement missing features,
- Help us [log bugs and open issues](https://github.com/bryphe/revery/issues/new).

## Special Thanks

`revery` would not be possible without a bunch of cool tech:
- [ocaml](https://ocaml.org) made these tools possible - thanks [Inria](http://gallium.inria.fr/) & [OCaml Labs](http://ocamllabs.io/)!
- [reasonml](https://reasonml.github.io) made revery possible - thanks @jordwalke!
- [flex](https://github.com/jordwalke/flex) by @jordwalke
- [briskml](https://github.com/briskml)
    - [brisk-reconciler](https://github.com/briskml/brisk-reconciler) - the "native React" implementation.
- [reason-glfw](https://github.com/bryphe/reason-glfw)
    - [GLFW](https://www.glfw.org)
    - [stb-image](https://github.com/nothings/stb)
- [reason-fontkit](https://github.com/bryphe/reason-fontkit)
    - [freetype2](https://www.freetype.org)
    - [harfbuzz](https://www.freedesktop.org/wiki/Software/HarfBuzz)
- [reason-gl-matrix](https://github.com/bryphe/reason-gl-matrix)
    - [gl-matrix](http://glmatrix.net)
    - [glm](https://glm.g-truc.net/0.9.9/index.html)
- [@reason-native/console](https://github.com/facebookexperimental/reason-native/tree/master/src/console)

`revery` was inspired by some __awesome projects:__
- [react-native](https://facebook.github.io/react-native/)
- [ReactMini](https://github.com/reasonml/reason-react/tree/master/ReactMini)
- [cuite](https://github.com/let-def/cuite)
- [wall](https://github.com/let-def/wall)
- [elm](https://elm-lang.org/)
