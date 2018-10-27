open Revery;
open Revery.Core;
open Revery.UI;

type item = {
    name: string,
    description: string
};

type state = {
    text: string
    items: list(item)
};

type actions =
| UpdateText(string)
| SetItems(list(item));

let initialState: world = {
    text: "",
    items: [
        { name: "a", "description of a"},
        { name: "b", "description of b"},
        { name: "c", "description of c"}
    ]
};

let reducer = (s: state, a: action) => {
    switch (a) {
    | UpdateText(s) => { ...s, text: s}
    | SetItems(i) => { ...s, items: i }
    };
};

let init = app => {

  let w = App.createWindow(app, "test");

  let ui = UI.create(w);

  let textHeaderStyle = Style.make(~backgroundColor=Colors.black, ~color=Colors.white, ~fontFamily="Roboto-Regular.ttf", ~fontSize=24, ());

  let smallerTextStyle = Style.make(~backgroundColor=Colors.black, ~color=Colors.white, ~fontFamily="Roboto-Regular.ttf", ~fontSize=12, ());

  Window.setRenderCallback(w, (state) => {
    UI.render(ui,
        <view style=(Style.make(~position=LayoutTypes.Absolute, ~bottom=10, ~top=10, ~left=10, ~right=10, ~backgroundColor=Colors.blue, ()))>
            <view style=(Style.make(~position=LayoutTypes.Absolute, ~bottom=0, ~width=10, ~height=10, ~backgroundColor=Colors.red, ())) />
            <image src="outrun-logo.png" style=(Style.make(~width=128, ~height=64, ())) />
            <text style=(textHeaderStyle)>"Hello World!"</text>
            <text style=(smallerTextStyle)>"Welcome to revery"</text>
            <view style=(Style.make(~width=25, ~height=25, ~backgroundColor=Colors.green, ())) />
        </view>);
  });
};

App.start(init, ~initialState=initialState, ~reducer=reducer);
