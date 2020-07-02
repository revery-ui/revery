open Revery;
open Revery.UI;
open Revery.UI.Components;

module FontComponent = {
  type state = {
    family: string,
    bold: bool,
    italic: bool,
  };

  let initialState: state = {family: "Arial", bold: false, italic: false};

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

    state;
  };

  let%component make = () => {
    let%hook (state, dispatch) = Hooks.reducer(~initialState, reducer);

    let fontExample =
      <Text
        text="Lorem ipsum dolor sit amet"
        fontFamily={Font.Family.system(state.family)}
        fontWeight={state.bold ? Font.Weight.Bold : Font.Weight.Normal}
        italic={state.italic}
        fontSize=24.
      />;

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
};
