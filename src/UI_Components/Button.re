open Revery_UI;
open Revery_Core;

type clickFunction = unit => unit;
let noop = () => ();

let textStyle = (~fontSize) =>
  Style.make(
    ~fontSize,
    ~fontFamily="Roboto-Regular.ttf",
    ~color=Colors.white,
    (),
  );

let containerStyle = (~width, ~height, ~disabled, ~color) =>
  Style.make(
    ~position=LayoutTypes.Relative,
    ~backgroundColor=disabled ? Colors.dimGrey : color,
    ~justifyContent=LayoutTypes.JustifyCenter,
    ~alignItems=LayoutTypes.AlignCenter,
    ~border=Style.Border.make(~width=1, ~color=Colors.white, ()),
    ~height,
    ~width,
    (),
  );

include (
          val component(
                (
                  render,
                  ~onClick: clickFunction=noop,
                  ~children,
                  ~title,
                  ~color=Colors.dodgerBlue,
                  ~fontSize=40,
                  ~width=300,
                  ~height=100,
                  ~disabled=false,
                  (),
                ) =>
                render(
                  () =>
                    <Clickable onClick>
                      <view
                        style={containerStyle(
                          ~width,
                          ~height,
                          ~disabled,
                          ~color,
                        )}>
                        <text style={textStyle(~fontSize)}> title </text>
                      </view>
                    </Clickable>,
                  ~children,
                )
              )
        );
