module ContainerComponent = Container;
open Revery_UI;
open Revery_Core;
open Revery_UI_Primitives;

module Hooks = Revery_UI_Hooks;

module Cursor = {
  type state = {
    time: Time.t,
    isOn: bool,
  };

  type action =
    | Reset
    | Tick(Time.t);

  let use = (~interval, ~isFocused) => {
    let%hook (state, dispatch) =
      Hooks.reducer(
        ~initialState={time: Time.zero, isOn: false}, (action, state) => {
        switch (action) {
        | Reset => {isOn: true, time: Time.zero}
        | Tick(increasedTime) =>
          let newTime = Time.(state.time + increasedTime);

          /* if newTime is above the interval a `Tick` has passed */
          newTime >= interval
            ? {isOn: !state.isOn, time: Time.zero}
            : {...state, time: newTime};
        }
      });

    let%hook () =
      Hooks.effect(
        OnMount,
        () => {
          let clear =
            Tick.interval(time => dispatch(Tick(time)), Time.ms(16));
          Some(clear);
        },
      );

    let cursorOpacity = isFocused && state.isOn ? 1.0 : 0.0;

    (cursorOpacity, () => dispatch(Reset));
  };
};

// TODO: remove after 4.08
module Option = {
  let value = (~default) =>
    fun
    | Some(x) => x
    | None => default;
};

type state = {
  isFocused: bool, // TODO: Violates single source of truth
  value: string,
  cursorPosition: int,
};

type changeEvent = {
  value: string,
  character: string,
  keycode: Key.Keycode.t,
  altKey: bool,
  ctrlKey: bool,
  shiftKey: bool,
  superKey: bool,
};

type action =
  | Focus
  | Blur
  | TextInput(string, int);

let getStringParts = (index, str) => {
  switch (index) {
  | 0 => ("", str)
  | _ =>
    let strBeginning =
      try(Str.string_before(str, index)) {
      | _ => str
      };
    let strEnd =
      try(Str.string_after(str, index)) {
      | _ => ""
      };
    (strBeginning, strEnd);
  };
};

let getSafeStringBounds = (str, cursorPosition, change) => {
  let nextPosition = cursorPosition + change;
  let currentLength = String.length(str);
  nextPosition > currentLength
    ? currentLength : nextPosition < 0 ? 0 : nextPosition;
};

let removeCharacterBefore = (word, cursorPosition) => {
  let (startStr, endStr) = getStringParts(cursorPosition, word);
  let nextPosition = getSafeStringBounds(startStr, cursorPosition, -1);
  let newString = Str.string_before(startStr, nextPosition) ++ endStr;
  (newString, nextPosition);
};

let removeCharacterAfter = (word, cursorPosition) => {
  let (startStr, endStr) = getStringParts(cursorPosition, word);
  let newString =
    startStr
    ++ (
      switch (endStr) {
      | "" => ""
      | _ => Str.last_chars(endStr, String.length(endStr) - 1)
      }
    );
  (newString, cursorPosition);
};

let addCharacter = (word, char, index) => {
  let (startStr, endStr) = getStringParts(index, word);
  (startStr ++ char ++ endStr, String.length(startStr) + 1);
};

let reducer = (action, state) =>
  switch (action) {
  | Focus => {...state, isFocused: true}
  | Blur => {...state, isFocused: false}
  | TextInput(value, cursorPosition) => {...state, value, cursorPosition}
  };

let defaultHeight = 50;
let defaultWidth = 200;
let inputTextMargin = 10;

let defaultStyles =
  Style.[
    color(Colors.black),
    width(defaultWidth),
    height(defaultHeight),
    border(
      /*
         The default border width should be 5% of the full input height
       */
      ~width=float_of_int(defaultHeight) *. 0.05 |> int_of_float,
      ~color=Colors.black,
    ),
    backgroundColor(Colors.transparentWhite),
  ];

let%component make =
              (
                ~style=defaultStyles,
                ~placeholderColor=Colors.grey,
                ~cursorColor=Colors.black,
                ~autofocus=false,
                ~placeholder="",
                ~onFocus=() => (),
                ~onBlur=() => (),
                ~onKeyDown=_ => (),
                ~onChange=(_, _) => (),
                ~value=?,
                ~cursorPosition=?,
                (),
              ) => {
  let%hook (state, dispatch) =
    Hooks.reducer(
      ~initialState={
        isFocused: false,
        value: Option.value(value, ~default=""),
        cursorPosition: Option.value(cursorPosition, ~default=0),
      },
      reducer,
    );

  let%hook (textRef, setTextRef) = Hooks.ref(None);

  let value = Option.value(value, ~default=state.value);

  let cursorPosition =
    Option.value(cursorPosition, ~default=state.cursorPosition);

  let%hook (cursorOpacity, resetCursor) =
    Cursor.use(~interval=Time.ms(500), ~isFocused=state.isFocused);

  let handleFocus = () => {
    resetCursor();
    onFocus();
    Sdl2.TextInput.start();
    dispatch(Focus);
  };

  let handleBlur = () => {
    resetCursor();
    onBlur();
    Sdl2.TextInput.stop();
    dispatch(Blur);
  };

  // TODO:This ought to be in the reducer, but since reducer calls are deferred
  // the ordering of side-effects can't be guaranteed.
  //
  // Refactor when https://github.com/briskml/brisk-reconciler/issues/54 has been fixed
  let update = (value, cursorPosition) => {
    onChange(value, cursorPosition);
    dispatch(TextInput(value, cursorPosition));
  };

  let handleTextInput = (event: NodeEvents.textInputEventParams) => {
    resetCursor();
    let (value, cursorPosition) =
      addCharacter(value, event.text, cursorPosition);
    update(value, cursorPosition);
  };

  let handleKeyDown = (event: NodeEvents.keyEventParams) => {
    resetCursor();
    onKeyDown(event);

    switch (event.keycode) {
    | v when Key.Keycode.left == v =>
      let cursorPosition = getSafeStringBounds(value, cursorPosition, -1);
      update(value, cursorPosition);

    | v when Key.Keycode.right == v =>
      let cursorPosition = getSafeStringBounds(value, cursorPosition, 1);
      update(value, cursorPosition);

    | v when Key.Keycode.delete == v =>
      let (value, cursorPosition) =
        removeCharacterAfter(value, cursorPosition);
      update(value, cursorPosition);

    | v when Key.Keycode.backspace == v =>
      let (value, cursorPosition) =
        removeCharacterBefore(value, cursorPosition);
      update(value, cursorPosition);

    | v when Key.Keycode.escape == v => Focus.loseFocus()

    | _ => ()
    };
  };

  let showPlaceholder = value == "";

  let allStyles =
    Style.(
      merge(
        ~source=[
          flexDirection(`Row),
          alignItems(`Center),
          justifyContent(`FlexStart),
          overflow(`Hidden),
          cursor(MouseCursors.text),
          ...defaultStyles,
        ],
        ~target=style,
      )
    );

  let viewStyles = Style.extractViewStyles(allStyles);

  let inputFontSize = Selector.select(style, FontSize, 18);
  let inputColor = Selector.select(style, Color, Colors.black);
  let inputFontFamily =
    Selector.select(style, FontFamily, "Roboto-Regular.ttf");

  let handleClick = (event: NodeEvents.mouseButtonEventParams) => {
    let rec offsetLeft = node => {
      let Dimensions.{left, _} = node#measurements();
      switch (node#getParent()) {
      | Some(parent) => left + offsetLeft(parent)
      | None => left
      };
    };

    let indexNearestOffset = offset => {
      open Revery_Draw;

      let rec loop = (i, last) =>
        if (i > String.length(value)) {
          i - 1;
        } else {
          let substring = String.sub(value, 0, i);
          let Text.{width, _} =
            Text.measure(
              ~fontSize=inputFontSize,
              ~fontFamily=inputFontFamily,
              substring,
            );

          if (width > offset) {
            let isCurrentNearest = width - offset < offset - last;
            isCurrentNearest ? i : i - 1;
          } else {
            loop(i + 1, width);
          };
        };

      loop(1, 0);
    };

    switch (textRef) {
    | Some(node) =>
      let offset = int_of_float(event.mouseX) - offsetLeft(node);
      let cursorPosition = indexNearestOffset(offset);
      resetCursor();
      update(value, cursorPosition);

    | None => ()
    };
  };

  let cursor = {
    let (startStr, _) = getStringParts(cursorPosition, value);
    let dimension =
      Revery_Draw.Text.measure(
        ~window=Revery_UI.getActiveWindow(),
        ~fontFamily=inputFontFamily,
        ~fontSize=inputFontSize,
        startStr,
      );
    let inputHeight = Selector.select(style, Height, defaultHeight);
    <View
      style=Style.[
        position(`Absolute),
        marginLeft(dimension.width + inputTextMargin + 1),
        marginTop((inputHeight - dimension.height) / 2),
      ]>
      <Opacity opacity=cursorOpacity>
        <ContainerComponent width=2 height=inputFontSize color=cursorColor />
      </Opacity>
    </View>;
  };

  let textComponent =
    <Text
      ref={node => setTextRef(Some(node))}
      text={showPlaceholder ? placeholder : value}
      style=Style.[
        color(showPlaceholder ? placeholderColor : inputColor),
        fontFamily(inputFontFamily),
        fontSize(inputFontSize),
        alignItems(`Center),
        justifyContent(`FlexStart),
        marginLeft(inputTextMargin),
      ]
    />;

  <Clickable
    onFocus=handleFocus
    onBlur=handleBlur
    componentRef={autofocus ? Focus.focus : ignore}
    onAnyClick=handleClick
    onKeyDown=handleKeyDown
    onTextInput=handleTextInput>
    <View style=viewStyles> cursor textComponent </View>
  </Clickable>;
};
