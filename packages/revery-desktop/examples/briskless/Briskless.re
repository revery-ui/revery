type state = {
  width: int,
  height: int,
  clicked: string,
};

let initialState = {width: 800, height: 600, clicked: ""};

let currentState = ref(initialState);

module Color = {
  let blue = 
                 Skia.Color.makeArgb(
                   0xFFl,
                   0x00l,
                   0x00l,
                   0xFFl,
                 );
  let red = Skia.Color.makeArgb(0xFFl, 0xFFl, 0x00l, 0x00l);
  let green = Skia.Color.makeArgb(0xFFl, 0x00l, 0xFFl, 0x00l);
  }

module JSXComponents = {
  open Primitives;

  let row = (~children as items, ()) => Widget.Container(Row, items);

  let box = (~color, ~children as child, ()) => Widget.Box({color, child});

  let text = (~text, ~children as _, ()) => Widget.Text(text);

  let clickable = (~onClick, ~children as child, ()) =>
    Widget.Clickable({onClick, child});

  let sizedBox = (~width, ~height, ~children as child, ()) =>
    Widget.ConstrainedBox({
      constraints: {
        minWidth: width,
        maxWidth: width,
        minHeight: height,
        maxHeight: height,
      },
      child,
    });

  let align = (~alignment, ~children as child, ()) =>
    Widget.Align(alignment, child);

  let padding = (~left=0, ~top=0, ~bottom=0, ~right=0, ~children as child, ()) =>
    Widget.Padding({left, top, bottom, right, child});
};

// APP

let setClicked = clicked => currentState := {...currentState^, clicked};

open JSXComponents;

let button = (~label, ~color, ~width=50, ~height=20, ~children as _, ()) =>
  <sizedBox width height>
       ...<clickable onClick={() => setClicked(label)}>
            ...<box color>
                 ...<text text=label />
               </box>
          </clickable>
     </sizedBox>;

let app = (~state, ~children as _, ()) =>
    <row>
      (
        `Weight(1),
        <clickable onClick={() => setClicked("red")}>
          ...<box color=Color.red>
               ...<align alignment=`topCenter>
                ...<padding top=10>
                    ...<button label="blue" color=Color.blue />
                </padding>
                  </align>
             </box>
        </clickable>,
      )
      (
        `Weight(1),
        <clickable onClick={() => setClicked("green")}>
          ...<box color=Color.green>
               ...<text text={state.clicked} />
             </box>
        </clickable>,
      )
    </row>;

let window =
  Window.create(
    ~title="Hello",
    ~width=initialState.width,
    ~height=initialState.height,
    ~onResize=
      Some(
        (width, height) => currentState := {...currentState^, width, height},
      ),
  );

let render = () => {
  Window.render(<app state=currentState^ />, window);
};

render();

Sdl2.renderLoop(() => {
  switch (Sdl2.Event.wait()) {
  | Ok(MouseButtonDown({x, y, _})) =>
    //      Printf.printf("mouse down: %n, %n\n%!", x, y);
    Window.handleClick(x, y, <app state=currentState^ />, window);
    render();

  | Ok(Quit) => exit(0)
  | Ok(WindowResized(event))
  | Ok(WindowSizeChanged(event)) =>
    //      print_endline("resizing");
    Option.iter(f => f(event.width, event.height), window.onResize);
    render();

  | _ => ()
  };

  false;
});
