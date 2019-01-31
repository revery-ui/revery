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

let selectedItemContainerStyles =
  Style.[
    flexDirection(`Row),
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
        marginHorizontal(2),
      ],
    )
  );

let noop = _item => ();

let component = React.component("Dropdown");
let make = (~items, ~onItemSelected, ()) =>
  component(slots => {
    let initialState = {items, selected: List.nth(items, 0), _open: false};

    let (state, dispatch, _slots: React.Hooks.empty) =
      React.Hooks.reducer(~initialState, reducer, slots);

    let longestItemText =
      List.nth(
        List.sort(
          (a, b) => String.length(b.label) - String.length(a.label),
          items,
        ),
        0,
      ).
        label;

    /* let textNode = (new textNode)(longestItemText);
       textNode#setStyle(
         Style.make(~fontFamily="Roboto-Regular.ttf", ~fontSize=20, ()),

       );  */
    /*
     Render text node to get measurements
     */
    /* Layout.layout(textNode, 1.0); */

    /* let dimensions = textNode#measurements();
       let containerWidth = dimensions.width; */

    let font =
      FontCache.load(
        "Roboto-Regular.ttf",
        int_of_float(float_of_int(20) *. 1.0),
      );

    let containerWidth =
      FontRenderer.measure(font, longestItemText).width + 40;

    let items =
      if (state._open) {
        List.map(
          _item =>
            <Clickable
              style=Style.[
                flexDirection(`Row),
                height(50),
                alignItems(`Center),
                justifyContent(`Center),
                backgroundColor(
                  _item == state.selected
                    ? Color.hex("#0078D7") : Colors.transparentWhite,
                ),
                borderBottom(
                  ~color=Colors.black,
                  ~width=float_of_int(50) *. 0.05 |> int_of_float,
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
        width(containerWidth),
      ]>
      <Clickable onClick={() => dispatch(ShowDropdown)}>
        <View style=selectedItemContainerStyles>
          <Text style=selectedItemStyles text={state.selected.label} />
          <Text
            style=Style.[
              fontSize(30),
              color(Colors.black),
              fontFamily("FontAwesome5FreeSolid.otf"),
              marginTop(15),
              marginHorizontal(2),
            ]
            text={||}
          />
        </View>
      </Clickable>
      <View
        style=Style.[
          position(`Absolute),
          top(50),
          backgroundColor(Colors.white),
          width(containerWidth),
          borderHorizontal(
            ~width=float_of_int(50) *. 0.05 |> int_of_float,
            ~color=Colors.black,
          ),
        ]>
        ...items
      </View>
    </View>;
  });

let createElement = (~items, ~onItemSelected=noop, ~children as _, ()) =>
  React.element(make(~items, ~onItemSelected, ()));
