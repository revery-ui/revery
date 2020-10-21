type state = {
  width: int,
  height: int,
};

let initialState = {width: 800, height: 600};

let currentState = ref(initialState);
let key = Brisk_reconciler.Key.create();

// APP

open Primitives;
open Components;

let app = (~state, ()) => {
  <Window
    key
    width={state.width}
    height={state.height}
    title={Printf.sprintf("Foo %ix%i", state.width, state.height)}
    onResize={(width, height) => currentState := {width, height}}>
    <row>
      <box
        label="red"
        color={Skia.Color.makeArgb(0xFFl, 0xFFl, 0x00l, 0x00l)}
      />
      <box
        label="green"
        color={Skia.Color.makeArgb(0xFFl, 0x00l, 0xFFl, 0x00l)}
      />
    </row>
  </Window>;
};

let currentElement = ref(<app state=initialState />);

let windows = ref([]);

let rendered =
  ref(
    Brisk_reconciler.RenderedElement.render(
      {
        node: (),
        insertNode: (~parent, ~child, ~position as _) => {
          print_endline("insert");
          windows := [child];
          parent;
        },
        deleteNode: (~parent, ~child as _, ~position as _) => {
          print_endline("delete");
          windows := [];
          parent;
        },
        moveNode: (~parent, ~child as _, ~from as _, ~to_ as _) => {
          print_endline("move");
          parent;
        },
      },
      currentElement^,
    ),
  );

let update = element => {
  rendered :=
    Brisk_reconciler.RenderedElement.update(
      ~previousElement=currentElement^,
      ~renderedElement=rendered^,
      element,
    );

  currentElement := element;
};

let render = () => {
  Printf.printf("render: Windows: %i\n%!", List.length(windows^));
  List.iter(Window.render, windows^);
};

Brisk_reconciler.RenderedElement.executeHostViewUpdates(rendered^) |> ignore;
render();

Sdl2.renderLoop(() => {
  switch (Sdl2.Event.wait()) {
  | Ok(Quit) => exit(0)
  | Ok(WindowResized(event))
  | Ok(WindowSizeChanged(event)) =>
    switch (
      List.find_opt(window => window.Window.id == event.windowID, windows^)
    ) {
    | Some(window) =>
      print_endline("resizing");
      Option.iter(f => f(event.width, event.height), window.onResize);
      //let nextElement = Brisk_reconciler.RenderedElement.flushPendingUpdates(rendered^);
      update(<app state=currentState^ />);
      Brisk_reconciler.RenderedElement.executeHostViewUpdates(rendered^)
      |> ignore;
      render();

    | None => ()
    }
  | _ => ()
  };

  false;
});
