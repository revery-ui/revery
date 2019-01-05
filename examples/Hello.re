open Revery;
open Revery.Core;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

module Logo = (
  val component((render, ~children, ()) =>
        render(
          () => {
            let (rotationY, _setRotationY) = useState(0.0);

            let rotation = 0.0;

            <view>
              <image
                src="outrun-logo.png"
                style={Style.make(
                  ~width=512,
                  ~height=256,
                  ~transform=[
                    RotateY(Angle.from_radians(rotationY)),
                    RotateX(Angle.from_radians(rotation)),
                  ],
                  (),
                )}
              />
            <Slider onValueChanged={(f) => _setRotationY(f)} />
            </view>;
          },
          ~children,
        )
      )
);

let init = app => {
  let win = App.createWindow(app, "Welcome to Revery!");

  let render = () =>
    <view
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
      <Logo />
    </view>;

  UI.start(win, render);
};

App.start(init);
