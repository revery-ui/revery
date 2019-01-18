open Revery;
open Revery.Core;
open Revery.UI;

/* Define our app state */

type item = {
  name: string,
  description: string,
};

type state = {
  text: string,
  items: list(item),
};

let createItem = (name, description) => {
  let ret: item = {name, description};
  ret;
};

let initialState: state = {
  text: "",
  items: [
    createItem("Item 1", "Item 1 Description"),
    createItem("Item 2", "Item 2 Description"),
    createItem("Item 3", "Item 3 Description"),
  ],
};

/* Define the operations we allow on our app state */

type action =
  | UpdateText(string)
  | SetItems(list(item))
  /* These actions should be baked into an input control! */
  | Backspace
  | ClearWord;

/* And the results of those actions on our app state, via a reducer */

let reducer = (s: state, a: action) =>
  switch (a) {
  | UpdateText(t) => {...s, text: s.text ++ t}
  | SetItems(i) => {...s, items: i}
  | Backspace =>
    let length = String.length(s.text);
    if (length > 0) {
      {...s, text: String.sub(s.text, 0, length - 1)};
    } else {
      s;
    };
  | ClearWord => {...s, text: ""}
  };

/* Helper method... There's no string.indexOf equivalent in the OCaml stdlib, surprisingly! */
let contains = (s1, s2) => {
  let re = Str.regexp_string(s2);

  try (
    {
      ignore(Str.search_forward(re, s1, 0));
      true;
    }
  ) {
  | Not_found => false
  };
};

/* A selector to get the set of items, based on our filter text */
let filterItems = (filterText: string, items: list(item)) => {
  let ft = String.lowercase_ascii(filterText);

  let f = i => {
    let normalizedName = String.lowercase_ascii(i.name);
    let normalizedDescription = String.lowercase_ascii(i.description);

    contains(normalizedName, ft) || contains(normalizedDescription, ft);
  };

  List.filter(f, items);
};

let init = app => {
  let width = 400;
  let height = 1;

  let w =
    App.createWindow(
      app,
      "test",
      ~createOptions={
        ...Window.defaultCreateOptions,
        decorated: false,
        visible: false,
        width,
        height,
      },
    );

  /* Figure out current monitor dimensions, so we can center it! */
  let monitor = Monitor.getPrimaryMonitor();
  let monitorSize = Monitor.getSize(monitor);

  Window.setPos(
    w,
    (monitorSize.width - width) / 2,
    (monitorSize.height - height) / 2,
  );
  Window.show(w);

  let textHeaderStyle =
    Style.make(
      ~backgroundColor=Colors.black,
      ~color=Colors.white,
      ~fontFamily="Roboto-Regular.ttf",
      ~fontSize=24,
      ~height=30,
      (),
    );

  /* let smallerTextStyle = Style.make(~backgroundColor=Colors.black, ~color=Colors.white, ~fontFamily="Roboto-Regular.ttf", ~fontSize=12, ()); */

  /* Listen to key press events, and coerce them into actions */
  let _ =
    Event.subscribe(w.onKeyPress, keyEvent =>
      App.dispatch(app, UpdateText(keyEvent.character))
    );

  /* Listen to key down events, and coerce them into actions, too */
  let _ =
    Event.subscribe(w.onKeyDown, keyEvent =>
      if (keyEvent.key == Key.KEY_BACKSPACE) {
        App.dispatch(app, Backspace);
      } else if (keyEvent.key == Key.KEY_H && keyEvent.ctrlKey) {
        App.dispatch(app, Backspace);
      } else if (keyEvent.key == Key.KEY_ESCAPE) {
        App.quit(0);
      } else if (keyEvent.key == Key.KEY_W && keyEvent.ctrlKey) {
        App.dispatch(app, ClearWord);
      }
    );

  let render = () => {
    let state = App.getState(app);

    let filteredItems = filterItems(state.text, state.items);
    let items =
      List.map(
        i => <Text style=textHeaderStyle text={i.name} />,
        filteredItems,
      );

    <View style={Style.make(~backgroundColor=Colors.blue, ~width, ())}>
      <View style={Style.make(~height=50, ())}>
        <Text style=textHeaderStyle text={state.text ++ "|"} />
      </View>
      <View style={Style.make()}> ...items </View>
    </View>;
  };

  UI.start(~createOptions={autoSize: true}, w, render);
};

App.startWithState(initialState, reducer, init);
