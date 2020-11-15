type state = {
  width: int,
  height: int,
  clicked: string,
};

let initialState = {width: 800, height: 600, clicked: ""};

let currentState = ref(initialState);

module Color = {
  let red = Skia.Color.makeArgb(0xFFl, 0xFFl, 0x00l, 0x00l);
  let green = Skia.Color.makeArgb(0xFFl, 0x00l, 0xFFl, 0x00l);
  let blue = Skia.Color.makeArgb(0xFFl, 0x00l, 0x00l, 0xFFl);
  let purple = Skia.Color.makeArgb(0xFFl, 0x80l, 0x00l, 0x80l);
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

let lorem = {|Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec blandit, lorem sit amet fermentum vehicula, ex velit aliquet elit, vel ultrices augue odio et urna. Morbi at leo pellentesque, tincidunt leo faucibus, consequat lacus. Pellentesque gravida, mauris in facilisis pellentesque, felis eros lobortis libero, sed volutpat enim tellus eget ante. Cras aliquet purus in nulla viverra pulvinar. Sed tincidunt venenatis pretium. Proin sagittis nisi auctor tempor gravida. Praesent placerat, mauris non luctus dapibus, lacus dui tincidunt felis, eu convallis urna velit sit amet odio. Nullam eu faucibus ligula. Maecenas elit dolor, lobortis eget urna at, bibendum porttitor purus. Nulla sodales vitae dolor at ultrices. Sed laoreet luctus ligula in ultricies. Curabitur a tellus ut nisl tincidunt tincidunt. Sed blandit purus imperdiet turpis euismod, rutrum egestas lorem tincidunt. Sed accumsan odio ligula, sed dapibus purus lobortis eu.

Vestibulum sollicitudin mollis finibus. Sed pretium vel est sit amet venenatis. Sed ac tincidunt dui. In tempor egestas tempor. Pellentesque a justo nec dolor tincidunt placerat. Nunc tristique sed quam eget tempus. Nunc consequat eu erat a consectetur. Morbi accumsan rhoncus nunc eget efficitur. Aenean viverra nisi nisi, vel ultrices lectus faucibus eu. Aliquam erat volutpat. Mauris nunc lectus, iaculis eu dui et, ullamcorper aliquet lorem. Proin viverra vitae nisi tincidunt facilisis. Pellentesque vulputate purus sed nisl placerat posuere. Phasellus purus sem, hendrerit sed turpis mollis, ultricies gravida tortor. Interdum et malesuada fames ac ante ipsum primis in faucibus.

Donec augue mauris, aliquet interdum augue in, posuere aliquet eros. Vivamus at auctor augue. In vitae iaculis neque, non facilisis ante. Aliquam condimentum lorem vel ex tristique elementum. Proin ultricies enim dapibus dui volutpat tempus. Maecenas fringilla non sapien id vestibulum. Nulla quis leo justo. Cras elementum lectus purus. Praesent at magna a est varius bibendum. Morbi fermentum erat sit amet sodales tristique.

Praesent tristique eu libero eu tempor. Duis quis metus vel sapien dignissim sagittis. Curabitur id scelerisque est. Cras aliquet lorem libero, et vestibulum purus tempor eu. In eget tortor massa. Integer dignissim facilisis feugiat. Duis enim dui, interdum id velit quis, consequat ullamcorper massa. Fusce blandit posuere bibendum. Donec vestibulum, tellus posuere suscipit ornare, sapien nisl viverra eros, nec lacinia odio urna id eros. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Nullam porta, dolor quis rhoncus vehicula, lacus tortor imperdiet dui, at ultricies ipsum arcu euismod ligula. Sed vel urna porta, facilisis metus nec, dapibus tellus. Suspendisse lobortis malesuada porttitor. Nam a dolor vel ex ullamcorper feugiat at ut ex. Duis tincidunt euismod nunc sit amet tempor. In interdum libero justo, vel sollicitudin dui fermentum at.

Nullam diam nisl, vehicula eu ante sagittis, tempor egestas ipsum. Maecenas efficitur scelerisque sapien et consectetur. Curabitur suscipit leo felis, eget pellentesque enim molestie in. Nulla facilisi. Ut efficitur vehicula metus, et pellentesque elit laoreet vitae. Integer pulvinar purus tincidunt, faucibus lectus quis, interdum libero. In pretium faucibus enim nec pretium.
|};

let app = (~state, ~children as _, ()) =>
  <row>
    (
      `Weight(1),
      <clickable onClick={() => setClicked("red")}>
        ...<box color=Color.red>
             ...<padding top=10>
                  ...<align alignment=`topCenter>
                       ...<button
                            label="blue kncdsl klcmds lkcmdsl"
                            color=Color.blue
                          />
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
    (
      `Weight(1),
      <clickable onClick={() => setClicked("purple")}>
        ...<box color=Color.purple>
             ...<text text=lorem fontFamily="Liberation Sans" size=12. />
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
