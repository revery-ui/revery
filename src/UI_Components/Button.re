open Revery_UI;
open Revery_Core;

let noop = () => ();

let textStyle = (~fontSize, ~fontFamily) =>
  Style.make(~fontSize, ~fontFamily, ~color=Colors.white, ());

let containerStyle = (~width, ~height, ~disabled, ~color) =>
  Style.make(
    ~position=Relative,
    ~backgroundColor=disabled ? Colors.dimGrey : color,
    ~justifyContent=JustifyCenter,
    ~alignItems=AlignCenter,
    ~border=Style.Border.make(~width=1, ~color=Colors.white, ()),
    ~height,
    ~width,
    (),
  );

include (
          val component(
                (
                  render,
                  ~children,
                  ~title,
                  ~onClick=noop,
                  ~color=Colors.dodgerBlue,
                  ~fontSize=40,
                  ~width=300,
                  ~height=100,
                  ~disabled=false,
                  ~fontFamily="Roboto-Regular.ttf",
                  (),
                ) =>
                render(
                  () =>
                    /* NOTE:If disabled the button should do nothing */
                    <Clickable onClick={disabled ? noop : onClick}>
                      <view
                        style={containerStyle(
                          ~width,
                          ~height,
                          ~disabled,
                          ~color,
                        )}>
                        <text style={textStyle(~fontSize, ~fontFamily)}>
                          title
                        </text>
                      </view>
                    </Clickable>,
                  ~children,
                )
              )
        );
