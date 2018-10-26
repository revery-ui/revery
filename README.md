# Revery

Build native, high-performance cross-platform desktop apps with Reason

## Building & Installing

### Install [esy](https://esy.sh/)

`esy` is like `npm` for native code. If you don't have it already, install it by:
```
npm install -g esy
```

### Building

- `esy install`
- `esy build`

### Running

### Running Tests

## Why?

Revery is a super-fast, _native_ Electron - with React, Redux, and a fast build system all-in-one!

Your apps are compiled to native code with the Reason / OCaml toolchain - with instant startup and performance comparable to native C code.

Today, [Electron](https://electronjs.org/) is one of the most popular tools for building desktop apps - using an HTML, JS, CSS stack. However, it has a heavy footprint in terms of both RAM and CPU - essentially packing an entire browser into the app. Even with these downsides, it has a lot of great aspects - it's the quickest way to build a cross-platform app & it provides a great development experience - as can be testified by its usage in VSCode, Discord, and Slack.

Revery is an experiment - can we provide a great developer experience without making sacrifices on performance? Revery is built on Reason, which is a javascript-like syntax on top of OCaml - meaning that Revery is accessible to JS developers. Reason / OCaml are _incredibly performant_ - on par with native C code in some cases! Revery makes full use of that speed, and, in addition, leverages GPU rendering.

### Design Decisions

- __Consistent cross-platform behavior__: A major value prop of Electron is that you can build for all platforms at once. You have great confidence as a developer that your app will look and work the same across different platforms. Revery is the same - aside from platform-specific behavior, if your app looks or behaves differently on another platform, that's a bug! As a consequence, Revery is like [flutter](TODO) in that it __does not use native widgets__. This means more work for us, but also that we have more predictable functionality cross-platform!
- __High performance__: Performance should be at the forefront - we need to develop and build benchmarks that help ensure top-notch performance and start-up time. This means we rely heavily on GPU acceleration.

## Why Not?

Revery is still highly experimental and is a WIP. Our hope is that it can become a viable platform for developing native desktop _and_ mobile apps - but we still have a lot of work ahead! 

## Roadmap

Some tentative work we need to do, in no particular order:

- UI Infrastructure
    - [x] Styles
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
    - Compilation to iOS
    - Compilation to Android
- Developer Experience
    - Hot reloading
    - Integrated debugger
    - Integrated performance profiler
- Example apps
    - Calculator
    - Todo List

## License

Revery is provided under the [MIT License](TODO).

## Contributing

We'd love your help! Please check out our [Contribution Guidelines](TODO). 

Some ideas for getting started:
- Help us build example apps
- Help us implement missing features
- Help us log bugs

## Special Thanks

`revery` would not be possible without a bunch of cool tech:
- [Reason](TODO) made revery possible - thanks @jordwalke!
- [flex] by @jordwalke
- [reason-reactify](TODO)
- [reason-glfw](TODO)
    - [GLFW](TODO) - 
    - [glmat](TODO)
    - [gl-matrix](TODO)
    - [stb-image](TODO)
- [reason-fontkit](TODO)
    - [freetype2](TODO)
    - [harfbuzz](TODO)

`revery` was inspired by __awesome projects:__
- [react-native]
- [briskml]
- [elm]



