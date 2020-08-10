open Revery;
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
  render: Window.t => element,
  source: string,
};

let examples = [
  {name: "Animation", render: _w => Hello.render(), source: "Hello.re"},
  {
    name: "Spring",
    render: _w => SpringExample.render(),
    source: "SpringExample.re",
  },
  {
    name: "CanQuit",
    render: _ => CanQuitExample.render(),
    source: "CanQuit.re",
  },
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
  {name: "Slider", render: _ => SliderExample.render(), source: "Slider.re"},
  {name: "Border", render: _ => Border.render(), source: "Border.re"},
  {
    name: "ScrollView",
    render: _ => ScrollViewExample.render(),
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
  {name: "Focus", render: _ => FocusExample.render(), source: "Focus.re"},
  {name: "Fonts", render: _ => FontsExample.render(), source: "Fonts.re"},
  {
    name: "Stopwatch",
    render: _ => Stopwatch.render(),
    source: "Stopwatch.re",
  },
  {
    name: "Native: File(s)/Folders(s)",
    render: _ => NativeFileExample.render(),
    source: "NativeFileExample.re",
  },
  {
    name: "Native: Notifications",
    render: _ => NativeNotificationExample.render(),
    source: "NativeNotificationExample.re",
  },
  {
    name: "Native: Icon Features",
    render: w => NativeIconExample.render(w),
    source: "NativeIconExample.re",
  },
  {
    name: "Input",
    render: _ => InputExample.render(),
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
    render: _ => AnalogClock.render(),
    source: "AnalogClock.re",
  },
  {
    name: "TodoMVC",
    render: _ => TodoExample.render(),
    source: "TodoExample.re",
  },
  {
    name: "Dropdown",
    render: _ => DropdownExample.render(),
    source: "DropdownExample.re",
  },
  {
    name: "Text",
    render: _w => TextExample.render(),
    source: "TextExample.re",
  },
  {
    name: "Hover Example",
    render: _ => HoverExample.render(),
    source: "HoverExample.re",
  },
  {
    name: "Canvas Example",
    render: _ => CanvasExample.render(),
    source: "CanvasExample.re",
  },
  {
    name: "SVG Example",
    render: _ => SVGExample.render(),
    source: "SVGExample.re",
  },
  {
    name: "Zoom Example",
    render: _ => ZoomExample.render(),
    source: "ZoomExample.re",
  },
  {
    name: "Nested Clickables",
    render: _ => NestedClickable.render(),
    source: "NestedClickable.re",
  },
  {
    name: "File Drag & Drop",
    render: _ => FileDragAndDrop.render(),
    source: "FileDragAndDrop.re",
  },
  {
    name: "Rich Text Example",
    render: _ => RichTextExample.render(),
    source: "RichTextExample.re",
  },
  {
    name: "Shell: Open URL",
    render: _ => URLFileOpen.render(),
    source: "URLFileOpen.re",
  },
  {
    name: "Window: Hit Tests/Zones",
    render: w => HitTests.render(w),
    source: "HitTests.re",
  },
  {
    name: "Window: Control",
    render: w => WindowControl.render(w),
    source: "WindowControl.re",
  },
  {
    name: "Markdown",
    render: _ => MarkdownExample.render(),
    source: "MarkdownExample.re",
  },
  {
    name: "ImageQuality",
    render: _ => ImageQualityExample.render(),
    source: "ImageQualityExample.re",
  },
];

let getExampleByName = name =>
  List.find(example => example.name == name, examples);

let noop = () => ();

module ExampleButton = {
  let make = (~isActive, ~name, ~onClick, ()) => {
    let highlightColor =
      isActive ? selectionHighlight : Colors.transparentWhite;

    let buttonOpacity = 1.0;
    let bgColor = isActive ? activeBackgroundColor : inactiveBackgroundColor;

    let wrapperStyle =
      Style.[
        borderLeft(~width=4, ~color=highlightColor),
        backgroundColor(bgColor),
      ];

    let textColor = isActive ? Colors.white : Colors.grey;
    let textHeaderStyle = Style.[color(textColor), margin(16)];

    <Opacity opacity=buttonOpacity>
      <Clickable style=wrapperStyle onClick>
        <Text style=textHeaderStyle text=name />
      </Clickable>
    </Opacity>;
  };
};

module ExampleHost = {
  let%component make = (~window, ~initialExample, ()) => {
    let%hook (selectedExample, setSelectedExample) =
      Hooks.state(getExampleByName(initialExample));

    let renderButton = example => {
      let isActive = example === selectedExample;
      let onClick = _ => {
        Window.setTitle(window, "Revery Example - " ++ example.name);

        prerr_endline("SOURCE FILE: " ++ example.source);
        notifyExampleSwitched(example.source);
        setSelectedExample(_ => example);
      };

      <ExampleButton isActive name={example.name} onClick />;
    };

    let buttons = List.map(renderButton, examples);

    let exampleView = selectedExample.render(window);

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
        <View> {buttons |> React.listToElement} </View>
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
        exampleView
      </View>
    </View>;
  };
};

let init = app => {
  Revery.App.initConsole();

  Timber.App.enable();
  Timber.App.setLevel(Timber.Level.perf);

  App.onIdle(app, () => prerr_endline("Idle!"))
  |> (ignore: Revery.App.unsubscribe => unit);
  App.onBeforeQuit(app, () => prerr_endline("Quitting!"))
  |> (ignore: Revery.App.unsubscribe => unit);

  let initialExample = ref("Animation");
  let decorated = ref(true);
  let forceScaleFactor = ref(None);
  let showFPSCounter = ref(false);
  Arg.parse(
    [
      ("--trace", Unit(() => Timber.App.setLevel(Timber.Level.trace)), ""),
      ("--no-decoration", Unit(() => decorated := false), ""),
      ("--example", String(name => initialExample := name), ""),
      (
        "--force-device-scale-factor",
        Float(scaleFactor => forceScaleFactor := Some(scaleFactor)),
        "",
      ),
      ("--show-fps", Unit(() => showFPSCounter := true), ""),
    ],
    _ => (),
    "There is only --trace, --example, --no-decoration, --show-fps, and --force-device-scale-factor",
  );
  let initialExample = initialExample^;

  let maximized = Environment.webGL;

  let windowWidth = 800;
  let windowHeight = 480;

  Console.log("Hello from example app");
  Console.log([1, 2, 3]);

  let window =
    App.createWindow(
      ~createOptions=
        WindowCreateOptions.create(
          ~width=windowWidth,
          ~height=windowHeight,
          ~maximized,
          ~titlebarStyle=Transparent,
          ~icon=Some("revery-icon.png"),
          ~decorated=decorated^,
          ~forceScaleFactor=forceScaleFactor^,
          ~acceleration=`Auto,
          (),
        ),
      app,
      "Welcome to Revery!",
    );
  if (showFPSCounter^) {
    Window.showFPSCounter(window);
  };

  if (Environment.webGL) {
    Window.maximize(window);
  };

  /* NOTE: If you want to use network-calls or other IO, uncomment this line */
  // let _startEventLoop = Revery_Lwt.startEventLoop();

  let _unsubscribe =
    Window.onFocusGained(window, () => Console.log("Focus gained"));
  let _unsubscribe =
    Window.onFocusLost(window, () => Console.log("Focus lost"));
  let _unsubscribe =
    Window.onMaximized(window, () => Console.log("Maximized!"));
  let _unsubscribe =
    Window.onFullscreen(window, () => Console.log("Fullscreen!"));
  let _unsubscribe =
    Window.onMinimized(window, () => Console.log("Minimized!"));
  let _unsubscribe =
    Window.onRestored(window, () => Console.log("Restored!"));

  let _unsubscribe =
    Window.onSizeChanged(window, ({width, height}) =>
      Console.log(Printf.sprintf("Size changed: %d x %d", width, height))
    );

  let _unsubscribe =
    Window.onMoved(window, ((x, y)) =>
      Console.log(Printf.sprintf("Moved: %d x %d", x, y))
    );

  Native.Tray.make(
    ~imagePath=Environment.getAssetPath("outrun-logo.png"),
    (),
  )
  |> ignore;

  Native.Tray.make() |> Native.Tray.setTitle(~text="Hello Revery!");

  let _renderFunction =
    UI.start(window, <ExampleHost window initialExample />);
  ();
};

Printexc.record_backtrace(true);
App.start(init);
