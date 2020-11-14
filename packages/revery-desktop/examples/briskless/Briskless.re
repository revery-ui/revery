type state = {
  width: int,
  height: int,
  clicked: string,
};

let initialState = {width: 800, height: 600, clicked: ""};

let currentState = ref(initialState);

module Color = {
  let blue = Skia.Color.makeArgb(0xFFl, 0x00l, 0x00l, 0xFFl);
  let red = Skia.Color.makeArgb(0xFFl, 0xFFl, 0x00l, 0x00l);
  let green = Skia.Color.makeArgb(0xFFl, 0x00l, 0xFFl, 0x00l);
  let white = Skia.Color.makeArgb(0xFFl, 0xFFl, 0xFFl, 0xFFl);
};

module JSXComponents = {
  open Primitives;

  let row = (~children as items, ()) => Widget.Container(Row, items);

  let box = (~color, ~children as child, ()) => Widget.Box({color, child});

  let text =
      (
        ~text,
        ~fontFamily,
        ~size,
        ~weight=500,
        ~italic=false,
        ~color=Color.white,
        ~lineHeight=1.4,
        ~children as _,
        (),
      ) =>
    Widget.Text({text, fontFamily, size, weight, italic, color, lineHeight});

  let clickable = (~onClick, ~children as child, ()) =>
    Widget.Clickable({onClick, child});

  let sizedBox = (~width, ~height, ~children as child, ()) =>
    Widget.ConstrainedBox({
      constraints: BoxConstraints.tight(~width, ~height),
      child,
    });

  let align = (~alignment, ~children as child, ()) =>
    Widget.Align(alignment, child);

  let padding = (~left=0, ~top=0, ~bottom=0, ~right=0, ~children as child, ()) =>
    Widget.Padding({left, top, bottom, right, child});
};

module State = {
  open Primitives;

  let create = initialValue => {
    let witness = Univ.Witness.create();
    (~children as render, ()) =>
      Widget.Effect({
        equal: (_, _) => false,
        mount: (~invalidate as _) => Univ.pack(witness, ref(initialValue)),
        render: (~invalidate, state) => {
          let state = Univ.unpack(witness, state) |> Option.get;
          let setState = value => {
            state.contents = value;
            invalidate();
          };

          render(state.contents, setState);
        },
        unmount: _state => (),
      });
  };
};

// APP

let setClicked = clicked => {
  Console.log(clicked);
  currentState := {...currentState^, clicked};
};

open JSXComponents;

let button = (~label, ~color, ~width=50, ~height=20, ~children as _, ()) =>
  <sizedBox width height>
    ...<clickable onClick={() => setClicked(label)}>
         ...<box color>
              ...<padding top=4 left=8>
                   ...<text text=label fontFamily="Liberation Sans" size=12. />
                 </padding>
            </box>
       </clickable>
  </sizedBox>;

let app = (~state, ~children as _, ()) =>
  <row>
    (
      `Weight(1),
      <clickable onClick={() => setClicked("red")}>
        ...<box color=Color.red>
             ...<padding top=10>
                  ...<align alignment=`topCenter>
                       ...<button label="blue" color=Color.blue />
                     </align>
                </padding>
           </box>
      </clickable>,
    )
    (
      `Weight(1),
      <clickable onClick={() => setClicked("green")}>
        ...<box color=Color.green>
             ...<align alignment=`center>
                  ...<text
                       text={state.clicked}
                       fontFamily="Liberation Sans"
                       size=12.
                     />
                </align>
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

let renderElement = () => {
  let spec = <app state=currentState^ />;
  Primitives.WidgetElement.fromSpec(spec);
};

let element = ref(renderElement());
Window.render(element.contents, window);

let render = () => {
  element := renderElement();
  Window.render(element.contents, window);
};

Sdl2.renderLoop(() => {
  switch (Sdl2.Event.wait()) {
  | Ok(MouseButtonDown({x, y, _})) =>
    //      Printf.printf("mouse down: %n, %n\n%!", x, y);
    Primitives.WidgetElement.handleClick(~x, ~y, element.contents);
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
