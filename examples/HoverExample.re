open Revery;
open Revery.UI;

type state = {
  parentBackground: Color.t,
  childOneBackground: Color.t,
  childTwoBackground: Color.t,
  childThreeBackground: Color.t,
  childFourBackground: Color.t,
};

type action =
  | SetParentBackground(Color.t)
  | SetChildOneBackground(Color.t)
  | SetChildTwoBackground(Color.t)
  | SetChildThreeBackground(Color.t)
  | SetChildFourBackground(Color.t);

let reducer = (action: action, state) => {
  switch (action) {
  | SetParentBackground(c) => {...state, parentBackground: c}
  | SetChildOneBackground(c) => {...state, childOneBackground: c}
  | SetChildTwoBackground(c) => {...state, childTwoBackground: c}
  | SetChildThreeBackground(c) => {...state, childThreeBackground: c}
  | SetChildFourBackground(c) => {...state, childFourBackground: c}
  };
};
module HoverExample = {
  let component = React.component("HoverExample");

  let initialState = {
    parentBackground: Colors.darkGray,
    childOneBackground: Colors.blanchedAlmond,
    childTwoBackground: Colors.blueViolet,
    childThreeBackground: Colors.burlyWood,
    childFourBackground: Colors.cornflowerBlue,
  };

  let createElement = (~children as _, ()) =>
    component(hooks => {
      let (state, dispatch, hooks) =
        Hooks.reducer(~initialState, reducer, hooks);

      (
        hooks,
        <View
          style=Style.[
            flexDirection(`Row),
            justifyContent(`Center),
            alignItems(`Center),
            width(500),
            height(500),
            backgroundColor(state.parentBackground),
          ]
          onMouseOver={_ => dispatch(SetParentBackground(Colors.lightGray))}
          onMouseOut={_ =>
            dispatch(SetParentBackground(initialState.parentBackground))
          }>
          <View
            style=Style.[
              flexDirection(`Row),
              flexWrap(`Wrap),
              justifyContent(`SpaceAround),
              alignItems(`Center),
            ]>
            <View
              style=Style.[
                width(175),
                height(175),
                backgroundColor(state.childOneBackground),
              ]
              onMouseEnter={_ =>
                dispatch(SetChildOneBackground(Colors.aquamarine))
              }
              onMouseLeave={_ =>
                dispatch(
                  SetChildOneBackground(initialState.childOneBackground),
                )
              }
            />
            <View
              style=Style.[
                width(175),
                height(175),
                backgroundColor(state.childTwoBackground),
              ]
              onMouseOver={_ =>
                dispatch(SetChildTwoBackground(Colors.forestGreen))
              }
              onMouseOut={_ =>
                dispatch(
                  SetChildTwoBackground(initialState.childTwoBackground),
                )
              }
            />
            <View
              style=Style.[
                width(175),
                height(175),
                backgroundColor(state.childThreeBackground),
              ]
              onMouseOver={_ =>
                dispatch(SetChildThreeBackground(Colors.darkSalmon))
              }
              onMouseOut={_ =>
                dispatch(
                  SetChildThreeBackground(initialState.childThreeBackground),
                )
              }
            />
            <View
              style=Style.[
                width(175),
                height(175),
                backgroundColor(state.childFourBackground),
              ]
              onMouseEnter={_ =>
                dispatch(SetChildFourBackground(Colors.tomato))
              }
              onMouseLeave={_ =>
                dispatch(
                  SetChildFourBackground(initialState.childFourBackground),
                )
              }
            />
          </View>
        </View>,
      );
    });
};

let render = () => {
  <View
    style=Style.[
      position(`Absolute),
      justifyContent(`Center),
      alignItems(`Center),
      bottom(0),
      top(0),
      left(0),
      right(0),
    ]>
    <HoverExample />
  </View>;
};
