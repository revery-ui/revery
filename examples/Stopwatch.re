/* open Revery; */
open Revery.Time;
open Revery.Core;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

module Clock = {
  type dispose = unit => unit;
  let noop = () => ();

  type state = {
    dispose: dispose,
    isRunning: bool,
    elapsedTime: Time.t,
  };

  type action = 
  | Start(dispose)
  | Stop
  | TimerTick(Time.t);

  /* let reducer = (_action: action, state: state) =>  { state }; */

  let reducer = (a, s) => switch(a) {
  | Start(f) => {
    {
        dispose: f,
        isRunning: true,
        elapsedTime: Seconds(0.),
    } 
  }    
  | Stop => {
    s.dispose();
    let ret = {
        dispose: noop,
        isRunning: false,
        elapsedTime: Seconds(0.),
    };
    ret;
  }
  | TimerTick(t) => {
    ...s,
    elapsedTime: s.isRunning ? Time.increment(s.elapsedTime, t) : s.elapsedTime
  }
  };

  let component = React.component("Clock");

  let make = () =>
    component(slots => {

      let (state, dispatch, slots) = React.Hooks.reducer(
          ~initialState={isRunning: false, dispose: noop, elapsedTime:Seconds(0.)},
          reducer, 
          slots);

      let _slots: React.Hooks.empty = React.Hooks.effect(OnMount, () => {
        Some(() => dispatch(Stop));
      }, slots);

    let clockWrapperStyle = Style.make(
        ~margin=20,
        ~width=150,
        ~borderBottom=Style.Border.make(~color=Colors.gray, ~width=2, ()),
        ()
    );


    let startStop = () => {
        switch (state.isRunning) {
        | true => dispatch(Stop)
        | false => {
            let dispose = Tick.interval((t) => dispatch(TimerTick(t)), Seconds(0.));
            dispatch(Start(dispose));
        };
        }
    };

    let style =
      Style.make(
        ~color=Colors.white,
        ~fontFamily="Roboto-Regular.ttf",
        ~fontSize=24,
        ~marginVertical=20,
        ~width=200,
        (),
      );

    let buttonText = state.isRunning ? "STOP" : "START";

    let marcherOpacity = state.isRunning ? 1.0 : 0.0;
    let getMarcherPosition = (t) => (sin(Time.to_float_seconds(t) *. 2. *. pi) /. 2.) +. 0.5;

    let marcherStyle = Style.make(
        ~position=LayoutTypes.Absolute,
        ~bottom=0,
        ~opacity=marcherOpacity,
        ~left=(int_of_float(getMarcherPosition(state.elapsedTime) *. 146.)),
        ~width=4,
        ~height=4,
        ~backgroundColor=Color.hex("#90f7ff"),
        (),
    );

  <View
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
        <View style=clockWrapperStyle>
        <Text style text={string_of_float(state.elapsedTime |> Time.to_float_seconds)} />
        <View style={marcherStyle} />
        </View>
        <Button title={buttonText} onClick=startStop />
      </View>;
    });

  let createElement = (~children as _, ()) => React.element(make());
};


let render = () => <Clock />
