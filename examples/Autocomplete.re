/* open Revery; */
/* open Revery.Core; */
/* open Revery.UI; */

/* /1* Define our app state *1/ */

/* type item = { */
/*   name: string, */
/*   description: string, */
/* }; */

/* type state = { */
/*   text: string, */
/*   items: list(item), */
/* }; */

/* let createItem = (name, description) => { */
/*   let ret: item = {name, description}; */
/*   ret; */
/* }; */

/* let initialState: state = { */
/*   text: "", */
/*   items: [ */
/*     createItem("Item 1", "Item 1 Description"), */
/*     createItem("Item 2", "Item 2 Description"), */
/*     createItem("Item 3", "Item 3 Description"), */
/*   ], */
/* }; */

/* /1* Define the operations we allow on our app state *1/ */

/* type action = */
/*   | UpdateText(string) */
/*   | SetItems(list(item)) */
/*   /1* These actions should be baked into an input control! *1/ */
/*   | Backspace */
/*   | ClearWord; */

/* /1* And the results of those actions on our app state, via a reducer *1/ */

/* let reducer = (s: state, a: action) => */
/*   switch (a) { */
/*   | UpdateText(t) => {...s, text: s.text ++ t} */
/*   | SetItems(i) => {...s, items: i} */
/*   | Backspace => */
/*     let length = String.length(s.text); */
/*     if (length > 0) { */
/*       {...s, text: String.sub(s.text, 0, length - 1)}; */
/*     } else { */
/*       s; */
/*     }; */
/*   | ClearWord => {...s, text: ""} */
/*   }; */

/* /1* Helper method... There's no string.indexOf equivalent in the OCaml stdlib, surprisingly! *1/ */
/* let contains = (s1, s2) => { */
/*   let re = Str.regexp_string(s2); */

/*   try ( */
/*     { */
/*       ignore(Str.search_forward(re, s1, 0)); */
/*       true; */
/*     } */
/*   ) { */
/*   | Not_found => false */
/*   }; */
/* }; */

/* /1* A selector to get the set of items, based on our filter text *1/ */
/* let filterItems = (filterText: string, items: list(item)) => { */
/*   let ft = String.lowercase_ascii(filterText); */

/*   let f = i => { */
/*     let normalizedName = String.lowercase_ascii(i.name); */
/*     let normalizedDescription = String.lowercase_ascii(i.description); */

/*     contains(normalizedName, ft) || contains(normalizedDescription, ft); */
/*   }; */

/*   List.filter(f, items); */
/* }; */

/* let init = app => { */
/*   let width = 400; */
/*   let height = 1; */

/*   let w = */
/*     App.createWindow( */
/*       app, */
/*       "test", */
/*       ~createOptions={ */
/*         ...Window.defaultCreateOptions, */
/*         decorated: false, */
/*         visible: false, */
/*         width, */
/*         height, */
/*       }, */
/*     ); */

/*   /1* Figure out current monitor dimensions, so we can center it! *1/ */
/*   let monitor = Monitor.getPrimaryMonitor(); */
/*   let monitorSize = Monitor.getSize(monitor); */

/*   Window.setPos( */
/*     w, */
/*     (monitorSize.width - width) / 2, */
/*     (monitorSize.height - height) / 2, */
/*   ); */
/*   Window.show(w); */

/*   let textHeaderStyle = */
/*     Style.make( */
/*       ~backgroundColor=Colors.black, */
/*       ~color=Colors.white, */
/*       ~fontFamily="Roboto-Regular.ttf", */
/*       ~fontSize=24, */
/*       ~height=30, */
/*       (), */
/*     ); */

/*   /1* let smallerTextStyle = Style.make(~backgroundColor=Colors.black, ~color=Colors.white, ~fontFamily="Roboto-Regular.ttf", ~fontSize=12, ()); *1/ */

/*   /1* Listen to key press events, and coerce them into actions *1/ */
/*   let _ = */
/*     Event.subscribe(w.onKeyPress, keyEvent => */
/*       App.dispatch(app, UpdateText(keyEvent.character)) */
/*     ); */

/*   /1* Listen to key down events, and coerce them into actions, too *1/ */
/*   let _ = */
/*     Event.subscribe(w.onKeyDown, keyEvent */
/*       => */
/*         if (keyEvent.key == Key.KEY_BACKSPACE) { */
/*           App.dispatch(app, Backspace); */
/*         } else if (keyEvent.key == Key.KEY_H && keyEvent.ctrlKey) { */
/*           App.dispatch(app, Backspace); */
/*         } else if (keyEvent.key == Key.KEY_ESCAPE) { */
/*           App.quit(0); */
/*         } else if (keyEvent.key == Key.KEY_W && keyEvent.ctrlKey) { */
/*           App.dispatch(app, ClearWord); */
/*         } */
/*       ); */

/*   let render = () => { */
/*     let state = App.getState(app); */

/*     let filteredItems = filterItems(state.text, state.items); */
/*     let items = */
/*       List.map( */
/*         i => <text style=textHeaderStyle> {i.name} </text>, */
/*         filteredItems, */
/*       ); */

/*     <view style={Style.make(~backgroundColor=Colors.blue, ~width, ())}> */
/*       <view style={Style.make(~height=50, ())}> */
/*         <text style=textHeaderStyle> {state.text ++ "|"} </text> */
/*       </view> */
/*       <view style={Style.make()}> ...items </view> */
/*     </view>; */
/*   }; */

/*   UI.start(~createOptions={autoSize: true}, w, render); */
/* }; */

/* App.startWithState(initialState, reducer, init); */
