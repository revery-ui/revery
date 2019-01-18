open Revery;
open Revery.Core;
/* open Revery.Math; */
open Revery.UI;
open Revery.UI.Components;

let backgroundColor = Color.hex("#212733");
let activeBackgroundColor = Color.hex("#2E3440");
let selectionHighlight = Color.hex("#90f7ff");

type example = {
    name: string,
    render: (Window.t) => React.syntheticElement,
};

type state = {
    examples: list(example),
    selectedExample: string,
};

let state: state = {
    examples: [{
       name: "Animation",
        render: (_w) => Hello.render(),
    }, {
        name: "Overflow",
        render: (_w) => Overflow.render(),
    }, {
        name: "Calculator",
        render: (w) => Calculator.render(w),
    }, {
        name: "Boxshadow",
        render: (_) => Boxshadow.render(),     
    }],
    selectedExample: "Animation"
};

let noop = () => ();

let getRenderFunctionSelector: (state) => Window.t => React.syntheticElement = (s: state) => {
    List.filter((x) => String.equal(x.name, s.selectedExample), state.examples)
                     |> List.hd
                     |> (a) => a.render;

    /* (_w) => { <view />}; */
}

module ExampleButton {

    let component = React.component("ExampleButton");

    let make = (~isActive: bool, ~name, ~onClick, ()) => component((_slots: React.Hooks.empty) => {
            let highlightColor = isActive ? selectionHighlight : Colors.transparentWhite;
            let opacity = isActive ? 1.0 : 0.6;

            let wrapperStyle =
              Style.make(
                ~opacity,
                ~borderLeft=Style.Border.make(~width=4, ~color=highlightColor, ()),
                ~backgroundColor=activeBackgroundColor,
                (),
              );

            let textHeaderStyle =
              Style.make(
                ~color=Colors.white,
                ~fontFamily="Roboto-Regular.ttf",
                ~fontSize=14,
                ~margin=16,
                (),
              );

            <View style={Style.make(~opacity, ())}>
            <Clickable style=wrapperStyle onClick>
              <Text style=textHeaderStyle text={name} />
            </Clickable>
            </View>;
        
    });

    let createElement = (~children as _, ~isActive, ~name, ~onClick, ()) => React.element(make(~isActive, ~name, ~onClick, ()));
};

type action =
| SelectExample(string);

let reducer = (s: state, a: action) => {

    let SelectExample(name) = a;

    let ret: state = {
        ...s,
        selectedExample: name,
    };
    ret;
};

let init = app => {

  let maximized = Environment.webGL;

  let win = App.createWindow(app, "Welcome to Revery!", ~createOptions={...Window.defaultCreateOptions, maximized});

  let render = () => {

    let s = App.getState(app);

    let renderButton = (x: example) => {
        let isActive = String.equal(x.name, s.selectedExample);
        <ExampleButton isActive={isActive} name={x.name} onClick={(_) => App.dispatch(app, SelectExample(x.name))} />
    };

    let buttons = List.map(renderButton, s.examples);

    let exampleRender = getRenderFunctionSelector(s);
    let example = exampleRender(win);

    <View
      onMouseWheel={(evt) => print_endline ("onMouseWheel: " ++ string_of_float(evt.deltaY))} 
      style={Style.make(
        ~position=LayoutTypes.Absolute,
        ~justifyContent=LayoutTypes.JustifyCenter,
        ~alignItems=LayoutTypes.AlignCenter,
        ~backgroundColor=backgroundColor,
        ~bottom=0,
        ~top=0,
        ~left=0,
        ~right=0,
        ~flexDirection=LayoutTypes.Row,
        (),
      )}>
      <View style={Style.make(
        ~position=LayoutTypes.Absolute, 
        ~top=0,
        ~left=0,
        ~width=175,
        ~bottom=0,
        ~backgroundColor,
        (),
      )}>
        ...buttons
      </View>
      <View style={Style.make(
        ~position=LayoutTypes.Absolute,
        ~top=0,
        ~left=175,
        ~right=0,
        ~bottom=0,
        ~backgroundColor=activeBackgroundColor,
        (),
      )}>
        example
      </View>
    </View>
  };

  if (Environment.webGL) {
    Window.maximize(win);
  }

  UI.start(win, render);
};

App.startWithState(state, reducer, init);
