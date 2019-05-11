open Revery;
open Revery.Math;
open Revery.UI;

module AnalogClock = {
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

  let component = React.component("AnalogClock");

  let createElement = (~children as _, ()) =>
    component(slots => {
      let (state, dispatch, slots) =
        React.Hooks.reducer(
          ~initialState={currentTime: Unix.time() |> Unix.localtime},
          reducer,
          slots,
        );

      let {hourDegrees, minuteDegrees, secondDegrees}: DegreeUtils.t =
        state.currentTime |> DegreeUtils.getDegreesFromTime;

      let slots =
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
          alignItems(`Center),
          bottom(0),
          justifyContent(`Center),
          left(0),
          position(`Absolute),
          right(0),
          top(0),
        ];

      let clockContainer =
        Style.[
          border(~width=4, ~color=Colors.white),
          height(300),
          position(`Relative),
          width(300),
        ];

      let clockPointer = Style.[];

      let hourStyle =
        Style.(
          merge(
            ~source=clockPointer,
            ~target=[
		      position(`Absolute),
              backgroundColor(Colors.white),
              height(90),
              width(4),
            ],
          )
        );

      let minutesStyle =
        Style.(
          merge(
            ~source=clockPointer,
            ~target=[
		      position(`Absolute),
              backgroundColor(Colors.white),
              height(120),
              width(4),
            ],
          )
        );

      let secondsStyle =
        Style.[
              backgroundColor(Colors.red),
              height(150),
              width(150),
            ];

      (
        slots,
        <View style=containerStyle>
          <View style=clockContainer>
              <Transform.Rotate value={Angle.from_degrees(secondDegrees *. 2.)}>
                <View style=secondsStyle />
              </Transform.Rotate>
                <View style=minutesStyle />
                <View style=hourStyle />
          </View>
        </View>,
      );
    });
};

let render = () => <AnalogClock />;
