open Revery.UI;
open Revery.Core;
open Revery.Math;

module WallClock = {
  module DegreeUtils = {
    type t = {
      hourDegrees: float,
      minuteDegrees: float,
      secondDegrees: float,
    };

    let numberToPercentage = (~number, ~max) => number /. max *. 100.;
    let percentageToDegrees = percentage => percentage *. 360. /. 100.;

    let getDegreesFromTime = ({tm_hour, tm_min, tm_sec, _}: Unix.tm) => {
      let hourDegrees =
        numberToPercentage(~number=float_of_int(tm_hour), ~max=12.)
        |> percentageToDegrees;

      let minuteDegrees =
        numberToPercentage(~number=float_of_int(tm_min), ~max=60.)
        |> percentageToDegrees;

      let secondDegrees =
        numberToPercentage(~number=float_of_int(tm_sec), ~max=60.)
        |> percentageToDegrees;

      {hourDegrees, minuteDegrees, secondDegrees};
    };
  };

  type action =
    | UpdateTime;

  type state = {currentTime: Unix.tm};

  let reducer = (action, _state) => {
    switch (action) {
    | UpdateTime => {currentTime: Unix.time() |> Unix.localtime}
    };
  };

  let component = React.component("WallClock");

  let make = () => {
    component(slots => {
      let (state, dispatch, slots) =
        React.Hooks.reducer(
          ~initialState={currentTime: Unix.time() |> Unix.localtime},
          reducer,
          slots,
        );

      let {hourDegrees, minuteDegrees, secondDegrees}: DegreeUtils.t =
        state.currentTime |> DegreeUtils.getDegreesFromTime;

      let _slots: React.Hooks.empty =
        React.Hooks.effect(
          OnMount,
          () => {
            let clear =
              Tick.interval(_ => dispatch(UpdateTime), Seconds(1.));
            Some(clear);
          },
          slots,
        );

      let containerStyle =
        Style.[
          justifyContent(`Center),
          alignItems(`Center),
          position(`Absolute),
          top(0),
          bottom(0),
          left(0),
          right(0),
        ];

      let countContainer =
        Style.[
          width(200),
          height(200),
          border(~width=4, ~color=Colors.yellow),
          position(`Relative),
        ];

      let clockPointer = Style.[position(`Absolute)];

      let hourStyle =
        Style.(
          merge(
            ~source=clockPointer,
            ~target=[
              width(2),
              height(60),
              top(70),
              left(100),
              backgroundColor(Colors.red),
              transform([
                Transform.Rotate(Angle.from_degrees(hourDegrees)),
                Transform.TranslateY(-50.),
              ]),
            ],
          )
        );

      let minutesStyle =
        Style.(
          merge(
            ~source=clockPointer,
            ~target=[
              width(2),
              height(100),
              top(50),
              left(100),
              backgroundColor(Colors.green),
              transform([
                Transform.Rotate(Angle.from_degrees(minuteDegrees)),
                Transform.TranslateY(-50.),
              ]),
            ],
          )
        );

      let secondsStyle =
        Style.(
          merge(
            ~source=clockPointer,
            ~target=[
              width(2),
              height(100),
              top(50),
              left(100),
              backgroundColor(Colors.blue),
              transform([
                Transform.Rotate(Angle.from_degrees(secondDegrees)),
                Transform.TranslateY(-50.),
              ]),
            ],
          )
        );

      <View style=containerStyle>
        <View style=countContainer>
          <View style=hourStyle />
          <View style=minutesStyle />
          <View style=secondsStyle />
        </View>
      </View>;
    });
  };

  let createElement = (~children as _, ()) => React.element(make());
};

let render = () => <WallClock />;