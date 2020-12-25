module ContainerComponent = Container;
open Revery_UI;
open Revery_Core;
open Revery_UI_Primitives;
open Revery_Font;

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
            Tick.interval(
              ~name="Revery:Input:Cursor Blink Interval",
              time => dispatch(Tick(time)),
              Time.ms(16),
            );
          Some(clear);
        },
      );

    let cursorOpacity = isFocused && state.isOn ? 1.0 : 0.0;

    (cursorOpacity, () => dispatch(Reset));
  };
};

type state = {
  value: string,
  cursorPosition: int,
};

type action =
  | TextInput(string, int);

let reducer = (action, _state) =>
  switch (action) {
  | TextInput(value, cursorPosition) => {value, cursorPosition}
  };

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

let insertString = (currentValue, insertion, index) => {
  let (startStr, endStr) = getStringParts(index, currentValue);
  (
    startStr ++ insertion ++ endStr,
    String.length(startStr) + String.length(insertion),
  );
};

module Constants = {
  let defaultHeight = 50;
  let defaultWidth = 200;
  let textMargin = 10;
  let cursorWidth = 2;
};

module Styles = {
  open Style;

  let defaultPlaceholderColor = Colors.grey;
  let defaultCursorColor = Colors.black;

  let default = [
    color(Colors.black),
    width(Constants.defaultWidth),
    height(Constants.defaultHeight),
    border(
      // The default border width should be 5% of the full input height
      ~width=float_of_int(Constants.defaultHeight) *. 0.05 |> int_of_float,
      ~color=Colors.black,
    ),
    backgroundColor(Colors.transparentWhite),
  ];

  let _all = (~style) =>
    merge(
      ~source=[
        flexDirection(`Row),
        alignItems(`Center),
        justifyContent(`FlexStart),
        overflow(`Hidden),
        cursor(MouseCursors.text),
        ...default,
      ],
      ~target=style,
    );

  let box = (~style) => extractViewStyles(_all(~style));

  let marginContainer = [
    flexDirection(`Row),
    alignItems(`Center),
    justifyContent(`FlexStart),
    marginLeft(Constants.textMargin),
    marginRight(Constants.textMargin),
    flexGrow(1),
  ];

  let cursor = (~offset) => [
    position(`Absolute),
    marginTop(2),
    transform(Transform.[TranslateX(float(offset))]),
  ];

  let textContainer = [flexGrow(1), overflow(`Hidden)];

  let text =
      (~showPlaceholder, ~scrollOffset, ~placeholderColor, ~color: Color.t) => [
    Style.color(showPlaceholder ? placeholderColor : color),
    alignItems(`Center),
    justifyContent(`FlexStart),
    textWrap(TextWrapping.NoWrap),
    transform(Transform.[TranslateX(float(- scrollOffset^))]),
  ];
};

let%component make =
              (
                ~style=Styles.default,
                ~fontFamily=Family.default,
                ~fontWeight=Weight.Normal,
                ~italic=false,
                ~fontSize=14.0,
                ~underlined=false,
                ~placeholderColor=Styles.defaultPlaceholderColor,
                ~cursorColor=Styles.defaultCursorColor,
                ~autofocus=false,
                ~placeholder="",
                ~onFocus=() => (),
                ~onBlur=() => (),
                ~onKeyDown=_ => (),
                ~onChange=(_, _) => (),
                ~value=?,
                ~cursorPosition=?,
                ~isPassword=false,
                (),
              ) => {
  // TODO: This ought to be a state hook not reducer but
  // a bug with stale hook setters might happen.
  //
  // Refactor when https://github.com/briskml/brisk-reconciler/issues/74 has been fixed
  let%hook (state, dispatch) =
    Hooks.reducer(
      ~initialState={
        value: Option.value(value, ~default=""),
        cursorPosition: Option.value(cursorPosition, ~default=0),
      },
      reducer,
    );
  let%hook textRef = Hooks.ref(None);
  let%hook scrollOffset = Hooks.ref(0);

  let color = Selector.select(style, Color, Colors.black);

  let value = Option.value(value, ~default=state.value);
  let showPlaceholder = value == "";
  let cursorPosition =
    Option.value(cursorPosition, ~default=state.cursorPosition)
    |> min(String.length(value));

  // TODO: Expose as argument
  let smoothing = Revery_Font.Smoothing.default;

  let measureTextWidth = text => {
    let dimensions =
      Revery_Draw.Text.dimensions(
        ~smoothing,
        ~italic,
        ~fontWeight,
        ~fontFamily,
        ~fontSize,
        text,
      );

    dimensions.width |> int_of_float;
  };

  let%hook clickableRef = Hooks.ref(None);
  let isFocused = () => {
    switch (clickableRef^) {
    | Some(node) => Focus.isFocused(node)
    | None => false
    };
  };

  let%hook (cursorOpacity, resetCursor) =
    Cursor.use(~interval=Time.ms(500), ~isFocused=isFocused());

  let () = {
    let cursorOffset =
      measureTextWidth(String.sub(value, 0, cursorPosition));

    switch (Option.bind(textRef^, r => r#getParent())) {
    | Some(containerNode) =>
      let container: Dimensions.t = containerNode#measurements();

      if (cursorOffset < scrollOffset^) {
        // out of view to the left, so align with left edge
        scrollOffset := cursorOffset;
      } else if (cursorOffset - scrollOffset^ > container.width) {
        // out of view to the right, so align with right edge
        scrollOffset := cursorOffset - container.width;
      };

    | None => ()
    };
  };

  let handleFocus = () => {
    resetCursor();
    onFocus();
    Sdl2.TextInput.start();
  };

  let handleBlur = () => {
    resetCursor();
    onBlur();
    Sdl2.TextInput.stop();
  };

  // TODO:This ought to be in the reducer, but since reducer calls are deferred
  // the ordering of side-effects can't be guaranteed.
  //
  // Refactor when https://github.com/briskml/brisk-reconciler/issues/54 has been fixed
  let update = (value, cursorPosition) => {
    onChange(value, cursorPosition);
    dispatch(TextInput(value, cursorPosition));
  };

  let paste = (currentValue, currentCursorPosition) => {
    switch (Sdl2.Clipboard.getText()) {
    | None => ()
    | Some(data) =>
      let (newValue, newCursorPosition) =
        insertString(currentValue, data, currentCursorPosition);
      update(newValue, newCursorPosition);
    };
  };

  let handleTextInput = (event: NodeEvents.textInputEventParams) => {
    resetCursor();
    let (value, cursorPosition) =
      insertString(value, event.text, cursorPosition);
    update(value, cursorPosition);
  };

  let handleKeyDown = (event: NodeEvents.keyEventParams) => {
    open Key;

    resetCursor();
    onKeyDown(event);

    let code = event.keycode;
    let super = Sdl2.Keymod.isGuiDown(event.keymod);
    let ctrl = Sdl2.Keymod.isControlDown(event.keymod);

    if (code == Keycode.left) {
      let cursorPosition = getSafeStringBounds(value, cursorPosition, -1);
      update(value, cursorPosition);
    } else if (code == Keycode.right) {
      let cursorPosition = getSafeStringBounds(value, cursorPosition, 1);
      update(value, cursorPosition);
    } else if (code == Keycode.delete) {
      let (value, cursorPosition) =
        removeCharacterAfter(value, cursorPosition);
      update(value, cursorPosition);
    } else if (code == Keycode.backspace) {
      let (value, cursorPosition) =
        removeCharacterBefore(value, cursorPosition);
      update(value, cursorPosition);
    } else if (code == Keycode.escape) {
      Focus.loseFocus();
    } else if (code == Keycode.v) {
      if (Environment.isMac && super || !Environment.isMac && ctrl) {
        paste(value, cursorPosition);
      };
    };
  };

  let handleClick = (event: NodeEvents.mouseButtonEventParams) => {
    switch (textRef^) {
    | Some(node) =>
      let sceneOffsets: Offset.t = node#getSceneOffsets();
      let textOffset =
        int_of_float(event.mouseX) - sceneOffsets.left + scrollOffset^;
      let cursorPosition =
        Revery_Draw.Text.indexNearestOffset(
          ~measure=measureTextWidth,
          value,
          textOffset,
        );

      resetCursor();
      update(value, cursorPosition);

    | None => ()
    };
  };

  let handleRef = node => {
    clickableRef := Some(node);
    if (autofocus) {
      Focus.focus(node);
    };
  };

  let cursor = () => {
    let (startStr, _) = getStringParts(cursorPosition, value);
    let textWidth = measureTextWidth(startStr);

    let offset = textWidth - scrollOffset^;

    <View style={Styles.cursor(~offset)}>
      <Opacity opacity=cursorOpacity>
        <ContainerComponent
          width=Constants.cursorWidth
          height={fontSize |> int_of_float}
          color=cursorColor
        />
      </Opacity>
    </View>;
  };

  let text =
    if (showPlaceholder) {
      placeholder;
    } else if (isPassword) {
      String.map(_ => '*', value);
    } else {
      value;
    };

  let text = () =>
    <Text
      ref={node => textRef := Some(node)}
      text
      fontFamily
      fontWeight
      italic
      fontSize
      underlined
      smoothing
      style={Styles.text(
        ~showPlaceholder,
        ~scrollOffset,
        ~placeholderColor,
        ~color,
      )}
    />;

  <Clickable
    onFocus=handleFocus
    onBlur=handleBlur
    componentRef=handleRef
    onAnyClick=handleClick
    onKeyDown=handleKeyDown
    onTextInput=handleTextInput>
    <View style={Styles.box(~style)}>
      <View style=Styles.marginContainer>
        <View style=Styles.textContainer> <text /> </View>
        <cursor />
      </View>
    </View>
  </Clickable>;
};
