open Revery_UI;
open Revery_Core;

type item = {
  value: string,
  label: string,
};

type items = list(item);

type state = {
  items,
  selected: item,
  _open: bool,
};

type action =
  | ShowDropdown
  | SelectItem(item);

let reducer = (action, state) =>
  switch (action) {
  | ShowDropdown => {...state, _open: !state._open}
  | SelectItem(item) => {...state, selected: item, _open: false}
  };

let containerStyles =
  Style.[position(`Relative), backgroundColor(Colors.white)];

let textStyles =
  Style.[
    fontFamily("Roboto-Regular.ttf"),
    fontSize(20),
    color(Colors.black),
  ];

let selectedItemContainerStyles =
  Style.[
    height(50),
    alignItems(`Center),
    justifyContent(`Center),
    border(
      ~width=float_of_int(50) *. 0.05 |> int_of_float,
      ~color=Colors.black,
    ),
  ];

let selectedItemStyles =
  Style.(
    merge(
      ~source=textStyles,
      ~target=[
        color(Colors.black),
        backgroundColor(Colors.transparentWhite),
        marginLeft(2),
      ],
    )
  );

let itemsContainerStyles =
  Style.[position(`Absolute), top(50), backgroundColor(Colors.white)];

let component = React.component("Dropdown");
let make = (~items, ()) =>
  component(slots => {
    let initialState = {items, selected: List.nth(items, 0), _open: false};

    let (state, dispatch, _slots: React.Hooks.empty) =
      React.Hooks.reducer(~initialState, reducer, slots);

    let items =
      if (state._open) {
        List.map(
          _item =>
            <Clickable
              style=Style.[
                backgroundColor(
                  _item == state.selected
                    ? Color.hex("#bfc1bb") : Colors.transparentWhite,
                ),
                height(50),
                alignItems(`Center),
                borderBottom(
                  ~color=Colors.black,
                  ~width=float_of_int(50) *. 0.05 |> int_of_float,
                ),
              ]
              onClick={() => dispatch(SelectItem(_item))}>
              <Text style=textStyles text={_item.label} />
            </Clickable>,
          state.items,
        );
      } else {
        [];
      };

    <View style=containerStyles>
      <Clickable onClick={() => dispatch(ShowDropdown)}>
        <View style=selectedItemContainerStyles>
          <Text style=selectedItemStyles text={state.selected.label} />
        </View>
      </Clickable>
      <View style=itemsContainerStyles> ...items </View>
    </View>;
  });

let createElement = (~items, ~children as _, ()) =>
  React.element(make(~items, ()));
