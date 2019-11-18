open Revery;
open Revery.UI;
open Revery.UI.Components;

module FontComponent = {
  type state = {
    family: string,
    mono: bool,
    bold: bool,
    italic: bool,
    resolvedFont: option(Font.t),
  };

  let initialState: state = {
    family: "Arial",
    mono: false,
    bold: false,
    italic: false,
    resolvedFont: None,
  };

  type actions =
    | SetBold(bool)
    | SetMono(bool)
    | SetItalic(bool)
    | SetFamily(string);

  let reducer = (action: actions, state: state) => {
    let state =
      switch (action) {
      | SetBold(v) => {...state, bold: v}
      | SetMono(v) => {...state, mono: v}
      | SetItalic(v) => {...state, italic: v}
      | SetFamily(v) => {...state, family: v}
      };

    let resolvedFont =
      Some(
        Font.find(
          ~weight=state.bold ? Font.Weight.Bold : Font.Weight.Normal,
          ~mono=state.mono,
          ~italic=state.italic,
          state.family,
        ),
      );

    switch (resolvedFont) {
    | Some(v) => print_endline("New font: " ++ Font.toString(v))
    | None => ()
    };

    {...state, resolvedFont};
  };

  let%component make = () => {
    let%hook (state, dispatch) = Hooks.reducer(~initialState, reducer);

    let fontExample =
      switch (state.resolvedFont) {
      | None => React.empty
      | Some(v) =>
        <Text
          text="Lorem ipsum dolor sit amet"
          style=Style.[fontFamily(v.path), fontSize(24)]
        />
      };

    <Container width=500 height=500>
      <Center>
        <Padding padding=10>
          <Row>
            <Input
              placeholder="Type font name"
              onChange={({value, _}) => dispatch(SetFamily(value))}
              value={state.family}
            />
          </Row>
        </Padding>
        <Padding padding=10>
          <Row>
            <Padding padding=16>
              <Checkbox
                checked={state.bold}
                onChange={() => dispatch(SetBold(!state.bold))}
              />
            </Padding>
            <Center>
              <Text
                text="Bold"
                style=Style.[
                  fontFamily("Roboto-Regular.ttf"),
                  fontSize(20),
                  width(150),
                ]
              />
            </Center>
          </Row>
        </Padding>
        <Padding padding=10>
          <Row>
            <Padding padding=16>
              <Checkbox
                checked={state.italic}
                onChange={() => dispatch(SetItalic(!state.italic))}
              />
            </Padding>
            <Center>
              <Text
                text="Italic"
                style=Style.[
                  fontFamily("Roboto-Regular.ttf"),
                  fontSize(20),
                  width(150),
                ]
              />
            </Center>
          </Row>
        </Padding>
        <Padding padding=10>
          <Row>
            <Padding padding=16>
              <Checkbox
                checked={state.mono}
                onChange={() => dispatch(SetMono(!state.mono))}
              />
            </Padding>
            <Center>
              <Text
                text="Mono"
                style=Style.[
                  fontFamily("Roboto-Regular.ttf"),
                  fontSize(20),
                  width(150),
                ]
              />
            </Center>
          </Row>
        </Padding>
        <Padding padding=16> <Row> fontExample </Row> </Padding>
      </Center>
    </Container>;
  };
};

let render = () => {
  <FontComponent />;
};
