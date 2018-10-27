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
    | UpdateText(t) => { ...s, text: s.text ++ t}
    | SetItems(i) => { ...s, items: i }
    };
};

let init = app => {

  let width = 800;
  let height = 600;

  let w = App.createWindow(app, "test", ~createOptions={
    ...Window.defaultCreateOptions,
    decorated: false,
    visible: false,
    width,
    height,
  });

  /* Figure out current monitor dimensions, so we can center it! */
  let monitor = Monitor.getPrimaryMonitor();
  let monitorSize = Monitor.getSize(monitor);

  /* IMO, it's more visually pleasing to have the UI pop up centered */
  let upwardsOffset = 100;

  Window.setPos(w, (monitorSize.width - width) / 2, ((monitorSize.height - height) / 2) - upwardsOffset);
  Window.show(w);

  let ui = UI.create(w);

  let textHeaderStyle = Style.make(~backgroundColor=Colors.black, ~color=Colors.white, ~fontFamily="Roboto-Regular.ttf", ~fontSize=24, ());

  /* let smallerTextStyle = Style.make(~backgroundColor=Colors.black, ~color=Colors.white, ~fontFamily="Roboto-Regular.ttf", ~fontSize=12, ()); */

  Window.setKeyPressCallback(w, (keyEvent) => {
    App.dispatch(app, UpdateText(keyEvent.character));
  });

  Window.setRenderCallback(w, () => {

    let state = App.getState(app);

    let items = List.map((i) => <text style=(textHeaderStyle)>{i.name}</text>, state.items);

    UI.render(ui,
        <view style=(Style.make(~position=LayoutTypes.Absolute, ~bottom=10, ~top=10, ~left=10, ~right=10, ~backgroundColor=Colors.blue, ()))>
            <view style=(Style.make(~position=LayoutTypes.Relative, ()))>...items</view>
            <text style=(textHeaderStyle)>{state.text}</text>
        </view>);
  });
};

App.startWithState(initialState, reducer, init);
