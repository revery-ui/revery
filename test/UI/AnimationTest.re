open Revery_UI;

open TestFramework;

let valueAt = (t, anim) => Animation.apply(Time.seconds(t), anim) |> fst;

// This is just to avoid having to coerce to float for every test if using normalizedTime
let floatify = Animation.tween(0., 1.);

describe("Animation", ({test, _}) => {
  test("tween", ({expect, _}) => {
    let anim = Animation.(animate(Time.seconds(1.)) |> tween(100., 110.));

    expect.float(valueAt(-2., anim)).toBeCloseTo(100.);
    expect.float(valueAt(0.25, anim)).toBeCloseTo(102.5);
    expect.float(valueAt(0.5, anim)).toBeCloseTo(105.);
    expect.float(valueAt(0.75, anim)).toBeCloseTo(107.5);
    expect.float(valueAt(2., anim)).toBeCloseTo(110.);
  });

  test("ease", ({expect, _}) => {
    let anim =
      Animation.(animate(Time.seconds(1.))
      |> ease(Easing.quadratic))
      |> floatify;

    expect.float(valueAt(-2., anim)).toBeCloseTo(0.);
    expect.float(valueAt(0.25, anim)).toBeCloseTo(0.0625);
    expect.float(valueAt(0.5, anim)).toBeCloseTo(0.25);
    expect.float(valueAt(0.75, anim)).toBeCloseTo(0.5625);
    expect.float(valueAt(2., anim)).toBeCloseTo(1.);
  });

  test("repeat", ({expect, _}) => {
    let anim = Animation.(animate(Time.seconds(1.)) |> repeat |> floatify);

    expect.float(valueAt(-2., anim)).toBeCloseTo(0.);
    expect.float(valueAt(0.75, anim)).toBeCloseTo(0.75);
    expect.float(valueAt(1.5, anim)).toBeCloseTo(0.5);
    expect.float(valueAt(4.66, anim)).toBeCloseTo(0.66);
  });

  test("delay", ({expect, _}) => {
    let anim = Animation.(animate(Time.seconds(1.)) |> delay(Time.seconds(1.)) |> floatify);
    
    expect.float(valueAt(-2., anim)).toBeCloseTo(0.);
    expect.float(valueAt(0.75, anim)).toBeCloseTo(0.);
    expect.float(valueAt(1.5, anim)).toBeCloseTo(0.5);
    expect.float(valueAt(4.66, anim)).toBeCloseTo(1.);
  });

  test("andThen", ({expect, _}) => {
    let first = Animation.(animate(Time.seconds(1.)) |> tween(0., 10.));
    let second = Animation.(animate(Time.seconds(1.)) |> tween(30., 20.));
    let anim = Animation.andThen(first, ~next=second);

    expect.float(valueAt(-2., anim)).toBeCloseTo(0.);
    expect.float(valueAt(0.75, anim)).toBeCloseTo(7.5);
    expect.float(valueAt(1.5, anim)).toBeCloseTo(25.);
    expect.float(valueAt(4.66, anim)).toBeCloseTo(20.);
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
