open Revery_UI;

open TestFramework;

describe("Animation", ({test, _}) => {
  test("simple animation", ({expect, _}) => {
    let myAnimation =
      Animation.(
        animate(Time.seconds(2.)) |> tween(0., 10.)
      );

    let (value, _state) = Animation.apply(Time.seconds(1.), myAnimation);
    expect.float(value).toBeCloseTo(5.);
  });

  test("animation with quadratic easing", ({expect, _}) => {
    let myAnimation =
      Animation.(
        animate(Time.seconds(1.)) |> ease(Easing.quadratic)
      );

    let (value, _state) = Animation.apply(Time.seconds(0.5), myAnimation);
    expect.float(value :> float).toBeCloseTo(0.25);
  });

  test("animation that repeats", ({expect, _}) => {
    let myAnimation =
      Animation.(
        animate(TIme.seconds(2.)) |> tween(0., 10.) |> repeat
      );

  test("animation with delay", ({expect, _}) => {
    let myAnimation =
      Animation.(
        animate(Time.seconds(2.)) |> tween(0., 10.) |> delay(Time.seconds(1.))
      );
    
    let (value, _state) = Animation.apply(Time.seconds(2.), myAnimation);
    expect.float(value).toBeCloseTo(5.);
  });

  test("chained animations", ({expect, _}) => {
    let first =
      Animation.(
        animate(Time.seconds(2.)) |> tween(0., 10.)
      );
    let second =
      Animation.(
        animate(Time.seconds(2.)) |> tween(10., 0.)
      );
    let combined = Animation.andThen(first, ~next=second);

    expect.float(fst(Animation.apply(Time.seconds(1.), combined))).toBeCloseTo(5.);
    expect.float(fst(Animation.apply(Time.seconds(3.), combined))).toBeCloseTo(5.);
  });

  // TODO
  /* test("alternating animation", ({expect, _}) => {
    let myAnimation =
      Animation.(
        animate(Time.seconds(2.) |> tween(0., 10.) |> alternatingRepeat
      );
    
    let (value, _state) = Animation.apply(Time.seconds(2.5), myAnimation);
    expect.float(value).toBeCloseTo(7.5);
  }); */
});
