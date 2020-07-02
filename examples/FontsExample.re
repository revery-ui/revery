/*open Revery;
  open Revery.UI;
  open Revery.UI.Components;

  module FontComponent = {
    type state = {
      family: string,
      bold: bool,
      italic: bool,
      resolvedFont: Font.Discovery.t
    };

    let initialState: state = {
      family: "Arial",
      bold: false,
      italic: false,
      resolvedFont: None,
    };

    type actions =
      | SetBold(bool)
      | SetItalic(bool)
      | SetFamily(string);

    let reducer = (action: actions, state: state) => {
      let state =
        switch (action) {
        | SetBold(v) => {...state, bold: v}
        | SetItalic(v) => {...state, italic: v}
        | SetFamily(v) => {...state, family: v}
        };

      let resolvedFont =
        Font.Discovery.find(
          ~weight=state.bold ? Font.Weight.Bold : Font.Weight.Normal,
          ~italic=state.italic,
          state.family,
        );

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
            fontFamily={Font.Family.fromFile(v.path)}
            fontSize=24.
          />
        };

      <Container width=500 height=500>
        <Center>
          <Padding padding=10>
            <Row>
              <Input
                placeholder="Type font name"
                onChange={(value, _) => dispatch(SetFamily(value))}
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
                <Text text="Bold" fontSize=20. style=Style.[width(150)] />
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
                <Text text="Italic" fontSize=20. style=Style.[width(150)] />
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
  };*/
