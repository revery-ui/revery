open Revery;
open Revery.Core;
open Revery.UI;

let init = app => {

  let w = App.createWindow(app, "test");

  let ui = UI.create(w);

  let numberLabel(radius, number) {
    let angle = float(number) *. 30.0 -. 90.0;
    let radians = x => x *. 3.14 /. 180.0;
    let x = int_of_float(radius *. cos(radians(angle)));
    let y = int_of_float(radius *. sin(radians(angle)));
    /* top = 245 b/c we have the weird invisible part of the hands & fonts draw from top down */
    let style = Style.make(~position=LayoutTypes.Absolute, ~top=245 + y, ~left=350 + x, ~backgroundColor=Colors.white, ~color=Colors.black, ~fontFamily="Roboto-Regular.ttf", ~fontSize=12, ());
    <text style> (string_of_int(number)) </text>;
  };

  let baseStyle = Style.make(~position=LayoutTypes.Absolute, ~top=150, ~left=350, ~backgroundColor=Colors.red);

  let hourHandStyle = clock =>
    baseStyle(~width=16, ~height=200, ~transform=[RotateZ(Angle.from_degrees(float(clock.Unix.tm_hour mod 12) *. 30.0))], ());

  let minuteHandStyle = clock =>
    baseStyle(~width=16, ~height=200, ~transform=[RotateZ(Angle.from_degrees(float(clock.Unix.tm_min) *. 6.0))], ());

  let secondHandStyle = clock =>
    baseStyle(~width=8, ~height=200, ~transform=[RotateZ(Angle.from_degrees(float(clock.Unix.tm_sec) *. 6.0))], ());

  Window.setShouldRenderCallback(w, () => true);

  Window.setRenderCallback(w, () => {
    let clock = Unix.time() |> Unix.localtime;
    
    UI.render(ui,
        <view style=(Style.make(~position=LayoutTypes.Absolute, ~bottom=50, ~top=50, ~left=50, ~right=50, ~backgroundColor=Colors.white, ()))>
            <image src="hour_hand.png" style=hourHandStyle(clock) />
            <image src="minute_hand.png" style=minuteHandStyle(clock) />
            <image src="second_hand.png" style=secondHandStyle(clock) />
            <view> ...(List.map(numberLabel(120.0), [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12])) </view>
        </view>);
  });
};

App.start(init);
