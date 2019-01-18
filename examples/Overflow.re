open Revery;
open Revery.UI;
open Revery.Core;
open Revery.UI.Components;

let containerStyle =
  Style.make(
    ~position=LayoutTypes.Absolute,
    ~top=0,
    ~bottom=0,
    ~left=0,
    ~right=0,
    ~alignItems=LayoutTypes.AlignCenter,
    ~justifyContent=LayoutTypes.JustifyCenter,
    ~flexDirection=LayoutTypes.Column,
    ~backgroundColor=Colors.black,
    (),
  );

let outerBox =
  Style.make(
    ~width=75,
    ~height=75,
    ~overflow=LayoutTypes.Hidden,
    ~backgroundColor=Colors.red,
    (),
  );

let innerBox =
  Style.make(
    ~width=150,
    ~height=150,
    ~backgroundColor=Color.rgba(0., 1.0, 0., 0.5),
    (),
  );

module Sample {
    let component = React.component("Sample");

    let make = () => component((slots) => {
            let (hidden, setHidden, _slots: React.Hooks.empty) = React.Hooks.state(false, slots);

            let outerStyle = {
              ...outerBox,
              overflow: hidden ? LayoutTypes.Hidden : LayoutTypes.Visible,
            };

            let onClick = _ => {
              setHidden(!hidden);
            };

            <View style=containerStyle>
              <View style=outerStyle> <View style=innerBox /> </View>
              <View style={Style.make(~marginTop=80, ())}>
                <Button
                  fontSize=20
                  height=45
                  title="Toggle overflow"
                  onClick
                />
              </View>
            </View>;
    });

    let createElement = (~children as _, ()) => React.element(make());
}

let init = app => {
  let win = App.createWindow(app, "Welcome to Revery!");

  UI.start(win, () => <Sample />);
};

App.start(init);
