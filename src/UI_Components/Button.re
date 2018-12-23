open Revery_UI;
open Revery_Core;

type clickFunction = unit => unit;
let noop = () => ();

/* TODO: Need to have font in the dir that contains this file,
   till I find how to import this correctly  */
let textStyle = (~fontSize) =>
  Style.make(
    ~fontSize,
    ~fontFamily="Roboto-Regular.ttf",
    ~color=Colors.white,
    (),
  );

let containerStyle = (~width, ~disabled) =>
  Style.make(
    ~position=LayoutTypes.Relative,
    ~backgroundColor=disabled ? Colors.dimGrey : Colors.dodgerBlue,
    ~justifyContent=LayoutTypes.JustifyCenter,
    ~alignItems=LayoutTypes.AlignCenter,
    ~border=Style.Border.make(~width=1, ~color=Colors.white, ()),
    ~height=100,
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
                  ~fontSize=40,
                  ~disabled=false,
                  (),
                ) =>
                render(
                  () => {
                    let width =
                      Monitor.getPrimaryMonitor()
                      |> Monitor.getSize
                      |> (({width, _}) => width * 30 / 100);
                    <Clickable onClick>
                      <view style={containerStyle(~width, ~disabled)}>
                        <text style={textStyle(~fontSize)}> title </text>
                      </view>
                    </Clickable>;
                  },
                  ~children,
                )
              )
        );
