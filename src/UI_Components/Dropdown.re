open Revery_UI;
open Revery_Core;

module FontRenderer = Revery_UI__FontRenderer;
module FontCache = Revery_UI__FontCache;

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

let textStyles =
  Style.[
    fontFamily("Roboto-Regular.ttf"),
    fontSize(20),
    color(Colors.black),
  ];

let selectedItemStyles =
  Style.(
    merge(
      ~source=textStyles,
      ~target=[
        color(Colors.black),
        backgroundColor(Colors.transparentWhite),
        marginLeft(5),
      ],
    )
  );

let noop = _item => ();

let component = React.component("Dropdown");
let make =
    (
      ~items,
      ~onItemSelected,
      ~width as w=200,
      ~height as h=50,
      ~children as _,
      (),
    ) =>
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
                flexDirection(`Row),
                height(h),
                alignItems(`Center),
                justifyContent(`FlexStart),
                backgroundColor(
                  _item == state.selected
                    ? Color.hex("#0078D7") : Colors.transparentWhite,
                ),
                borderBottom(
                  ~color=Colors.black,
                  ~width=float_of_int(h) *. 0.05 |> int_of_float,
                ),
              ]
              onClick={() => {
                dispatch(SelectItem(_item));
                onItemSelected(_item);
              }}>
              <Text style=textStyles text={_item.label} />
            </Clickable>,
          state.items,
        );
      } else {
        [];
      };

    <View
      style=Style.[
        position(`Relative),
        backgroundColor(Colors.white),
        width(w),
      ]>
      <Clickable onClick={() => dispatch(ShowDropdown)}>
        <View
          style=Style.[
            flexDirection(`Row),
            height(h),
            justifyContent(`SpaceBetween),
            border(
              ~width=float_of_int(h) *. 0.05 |> int_of_float,
              ~color=Colors.black,
            ),
          ]>
          <View
            style=Style.[
              width(float_of_int(w) *. 0.8 |> int_of_float),
              justifyContent(`Center),
              overflow(LayoutTypes.Hidden),
            ]>
            <Text style=selectedItemStyles text={state.selected.label} />
          </View>
          <Text
            style=Style.[
              fontSize(30),
              color(Colors.black),
              fontFamily("FontAwesome5FreeSolid.otf"),
              marginTop(15),
              marginRight(5),
              alignSelf(`Center),
            ]
            text={||}
          />
        </View>
      </Clickable>
      <View
        style=Style.[
          position(`Absolute),
          top(h),
          backgroundColor(Colors.white),
          borderHorizontal(
            ~width=float_of_int(h) *. 0.05 |> int_of_float,
            ~color=Colors.black,
          ),
          overflow(LayoutTypes.Hidden),
        ]>
        ...items
      </View>
    </View>;
  });

let createElement =
    (~items, ~onItemSelected=noop, ~width=200, ~height=50, ~children, ()) =>
  React.element(
    make(~items, ~onItemSelected, ~width, ~height, ~children, ()),
  );
