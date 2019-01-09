open Revery;
open Revery.Core;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

let init = app => {
  let win = App.createWindow(app, "Welcome to Revery!");

  let render = () =>
    <view
      onMouseWheel={(evt) => print_endline ("onMouseWheel: " ++ string_of_float(evt.deltaY))} 
      style={Style.make(
        ~position=LayoutTypes.Absolute,
        ~justifyContent=LayoutTypes.JustifyCenter,
        ~alignItems=LayoutTypes.AlignCenter,
        ~bottom=0,
        ~top=0,
        ~left=0,
        ~right=0,
        (),
      )}>
      <view
        ref={r => print_endline("View internal id:" ++ string_of_int(r#getInternalId()))}
        style={Style.make(~flexDirection=Row, ~alignItems=AlignFlexEnd, ())}>
      </view>
      <SimpleButton />
    </view>;

  UI.start(win, render);
};

App.start(init);
