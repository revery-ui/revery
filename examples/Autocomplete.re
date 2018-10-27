open Revery;
open Revery.Core;
open Revery.UI;

type item = {
    name: string,
    description: string
};

type state = {
    text: string,
    items: list(item)
};

type action =
| UpdateText(string)
| SetItems(list(item));

let createItem = (name, description) => {
    let ret: item = {name, description}
    ret;
}

let initialState: state = {
    text: "",
    items: [
        createItem("Item 1", "Item 1 Description"),
        createItem("Item 2", "Item 2 Description"),
        createItem("Item 3", "Item 3 Description")
    ]
};

let update = (a) => a;

let reducer = (s: state, a: action) => {
    switch (a) {
    | UpdateText(t) => { ...s, text: t}
    | SetItems(i) => { ...s, items: i }
    };
};

let init = app => {

  let w = App.createWindow(app, "test");

  let ui = UI.create(w);

  let textHeaderStyle = Style.make(~backgroundColor=Colors.black, ~color=Colors.white, ~fontFamily="Roboto-Regular.ttf", ~fontSize=24, ());

  let smallerTextStyle = Style.make(~backgroundColor=Colors.black, ~color=Colors.white, ~fontFamily="Roboto-Regular.ttf", ~fontSize=12, ());

  Window.setRenderCallback(w, () => {

    let state = App.getState(app);

    let items = List.map((i) => <text style=(textHeaderStyle)>{i.name}</text>, state.items);

    UI.render(ui,
        <view style=(Style.make(~position=LayoutTypes.Absolute, ~bottom=10, ~top=10, ~left=10, ~right=10, ~backgroundColor=Colors.blue, ()))>
            <view style=(Style.make(~position=LayoutTypes.Absolute, ~bottom=0, ~width=10, ~height=10, ~backgroundColor=Colors.red, ())) />
            <image src="outrun-logo.png" style=(Style.make(~width=128, ~height=64, ())) />
            <text style=(textHeaderStyle)>"Hello World!"</text>
            <text style=(smallerTextStyle)>"Welcome to revery"</text>
            <view>...items</view>
            <view style=(Style.make(~width=25, ~height=25, ~backgroundColor=Colors.green, ())) />
        </view>);
  });
};

App.startWithState(initialState, reducer, init);
