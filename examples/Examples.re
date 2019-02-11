open Revery;
open Revery.Core;
/* open Revery.Math; */
open ExampleStubs;

module SliderExample = Slider;
module ScrollViewExample = ScrollView;

open Revery.UI;
open Revery.UI.Components;

let bgColor = Color.hex("#212733");
let activeBackgroundColor = Color.hex("#2E3440");
let inactiveBackgroundColor = Color.hex("#272d39");
let selectionHighlight = Color.hex("#90f7ff");

type example = {
  name: string,
  render: Window.t => React.syntheticElement,
  source: string,
};

type state = {
  examples: list(example),
  selectedExample: string,
};

let state: state = {
  examples: [
    {name: "Animation", render: _w => Hello.render(), source: "Hello.re"},
    {
      name: "Button",
      render: _ => DefaultButton.render(),
      source: "DefaultButton.re",
    },
    {
      name: "Checkbox",
      render: _ => CheckboxExample.render(),
      source: "CheckboxExample.re",
    },
    {
      name: "Slider",
      render: _ => SliderExample.render(),
      source: "Slider.re",
    },
    {name: "Border", render: _ => Border.render(), source: "Border.re"},
    {
      name: "ScrollView",
      render: _w => ScrollViewExample.render(),
      source: "ScrollView.re",
    },
    {
      name: "Calculator",
      render: w => Calculator.render(w),
      source: "Calculator.re",
    },
    {name: "Flexbox", render: _ => Flexbox.render(), source: "Flexbox.re"},
    {
      name: "Box Shadow",
      render: _ => Boxshadow.render(),
      source: "Boxshadow.re",
    },
    {name: "Focus", render: _ => Focus.render(), source: "Focus.re"},
    {
      name: "Stopwatch",
      render: _ => Stopwatch.render(),
      source: "Stopwatch.re",
    },
    {name: "Native", render: w => Native.render(w), source: "Native.re"},
    {
      name: "Input",
      render: w => InputExample.render(w),
      source: "InputExample.re",
    },
    {
      name: "Radio Button",
      render: _ => RadioButtonExample.render(),
      source: "RadioButtonExample.re",
    },
    {
      name: "Game Of Life",
      render: _ => GameOfLife.render(),
      source: "GameOfLife.re",
    },
    {
      name: "Screen Capture",
      render: w => ScreenCapture.render(w),
      source: "ScreenCapture.re",
    },
    {
      name: "Tree View",
      render: w => TreeView.render(w),
      source: "TreeView.re",
    },
    {
      name: "Analog Clock",
      render: _w => AnalogClock.render(),
      source: "AnalogClock.re",
    },
    {
      name: "TodoMVC",
      render: w => TodoExample.render(w),
      source: "TodoExample.re",
    },
  ],
  selectedExample: "Animation",
};

let getExampleByName = (state: state, example: string) => {
  List.filter(x => String.equal(x.name, example), state.examples) |> List.hd;
};

let getSourceForSample = (state: state, example: string) => {
  getExampleByName(state, example) |> (s => s.source);
};

let noop = () => ();

let getRenderFunctionSelector: (state, Window.t) => React.syntheticElement =
  (s: state) => getExampleByName(s, s.selectedExample) |> (a => a.render);

module ExampleButton = {
  let component = React.component("ExampleButton");

  let make = (~isActive: bool, ~name, ~onClick, ()) =>
    component((_slots: React.Hooks.empty) => {
      let highlightColor =
        isActive ? selectionHighlight : Colors.transparentWhite;

      let buttonOpacity = 1.0;
      let bgColor = isActive ? activeBackgroundColor : inactiveBackgroundColor;

      let wrapperStyle =
        Style.[
          opacity(buttonOpacity),
          borderLeft(~width=4, ~color=highlightColor),
          backgroundColor(bgColor),
        ];

      let textColor = isActive ? Colors.white : Colors.grey;
      let textHeaderStyle =
        Style.[
          color(textColor),
          fontFamily("Roboto-Regular.ttf"),
          fontSize(14),
          margin(16),
        ];

      <View style=[Style.opacity(buttonOpacity)]>
        <Clickable style=wrapperStyle onClick>
          <Text style=textHeaderStyle text=name />
        </Clickable>
      </View>;
    });

  let createElement = (~children as _, ~isActive, ~name, ~onClick, ()) =>
    React.element(make(~isActive, ~name, ~onClick, ()));
};

type action =
  | SelectExample(string);

let reducer = (s: state, a: action) => {
  switch (a) {
  | SelectExample(name) => {...s, selectedExample: name}
  };
};

let init = app => {
  let maximized = Environment.webGL;

  let dimensions: Monitor.size =
    Monitor.getPrimaryMonitor() |> Monitor.getSize;

  let windowWidth = dimensions.width / 2;
  let windowHeight = dimensions.height / 2;

  let win =
    App.createWindow(
      app,
      "Welcome to Revery!",
      ~createOptions={
        ...Window.defaultCreateOptions,
        width: windowWidth,
        height: windowHeight,
        maximized,
      },
    );

  let render = () => {
    let s = App.getState(app);

    let renderButton = (x: example) => {
      let isActive = String.equal(x.name, s.selectedExample);
      <ExampleButton
        isActive
        name={x.name}
        onClick={_ => {
          /*
           * TEMPORARY WORKAROUND: The animations don't always get stopped when switching examples,
           * tracked by briskml/brisk-reconciler#8. We can remove this once it's fixed!
           */
          Animated.cancelAll();

          let sourceFile = getSourceForSample(s, x.name);
          notifyExampleSwitched(sourceFile);
          App.dispatch(app, SelectExample(x.name));
        }}
      />;
    };

    let buttons = List.map(renderButton, s.examples);

    let exampleRender = getRenderFunctionSelector(s);
    let example = exampleRender(win);

    <View
      onMouseWheel={_evt => ()}
      style=Style.[
        position(`Absolute),
        justifyContent(`Center),
        alignItems(`Center),
        backgroundColor(bgColor),
        bottom(0),
        top(0),
        left(0),
        right(0),
        flexDirection(`Row),
      ]>
      <ScrollView
        style=Style.[
          position(`Absolute),
          top(0),
          left(0),
          width(175),
          bottom(0),
          backgroundColor(bgColor),
        ]>
        <View> ...buttons </View>
      </ScrollView>
      <View
        style=Style.[
          position(`Absolute),
          top(0),
          left(175),
          right(0),
          bottom(0),
          backgroundColor(activeBackgroundColor),
        ]>
        example
      </View>
    </View>;
  };

  if (Environment.webGL) {
    Window.maximize(win);
  } else {
    let xPosition = (dimensions.width - windowWidth) / 2;
    let yPosition = (dimensions.height - windowHeight) / 2;
    Window.setPos(win, xPosition, yPosition);
  };

  UI.start(win, render);
};

App.startWithState(state, reducer, init);