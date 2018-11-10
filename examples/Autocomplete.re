open Revery;
open Revery.Core;
open Revery.UI;

/* Define our app state */

type item = {
    name: string,
    description: string
};

type state = {
    text: string,
    items: list(item)
};

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

/* Define the operations we allow on our app state */

type action =
| UpdateText(string)
| SetItems(list(item));


/* And the results of those actions on our app state, via a reducer */

let reducer = (s: state, a: action) => {
    switch (a) {
    | UpdateText(t) => { ...s, text: s.text ++ t}
    | SetItems(i) => { ...s, items: i }
    };
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
}

/* A selector to get the set of items, based on our filter text */
let filterItems = (filterText: string, items: list(item)) => {
    let ft = String.lowercase_ascii(filterText);

    let f = (i) => {
        let normalizedName = String.lowercase_ascii(i.name);
        let normalizedDescription = String.lowercase_ascii(i.description);
        
        contains(normalizedName, ft) || contains(normalizedDescription, ft)
    };

    List.filter(f, items);
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

  /* Listen to key press events, and coerce them into actions */
  let _ = Event.subscribe(w.onKeyPress, (keyEvent) => {
    print_endline("CHARACTER: " ++ keyEvent.character);
     App.dispatch(app, UpdateText(keyEvent.character));
  })

  /* Render function - where the magic happens! */
  Window.setRenderCallback(w, () => {
    let state = App.getState(app);

    let filteredItems = filterItems(state.text, state.items);
    let items = List.map((i) => <text style=(textHeaderStyle)>{i.name}</text>, filteredItems);

    print_endline ("NEW STATE TEXT: " ++ state.text);

    UI.render(ui,
        <view style=(Style.make(~position=LayoutTypes.Absolute, ~bottom=10, ~top=10, ~left=10, ~right=10, ~backgroundColor=Colors.blue, ()))>
            <view style=(Style.make(~position=LayoutTypes.Relative, ()))>...items</view>
            <text style=(textHeaderStyle)>{state.text}</text>
        </view>);
  });
};

App.startWithState(initialState, reducer, init);
