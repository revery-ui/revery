open Revery_Core;
open Revery_UI;

open TestFramework;

// This is just to avoid having to coerce to float for every test that uses NormalizedTime.t
let floatify = Animation.tween(0., 1.);

describe("Animation", ({describe, _}) => {
  describe("const", ({test, _}) => {
    assertMany(
      "examples",
      test,
      Animation.[
        const(3) |> valueAt(Time.zero) == 3,
        const(3) |> valueAt(Time.seconds(-42.)) == 3,
        const(3) |> valueAt(Time.seconds(42.)) == 3,
        const(3) |> stateAt(Time.seconds(-42.)) == Complete(Time.zero),
        const(3) |> stateAt(Time.seconds(42.)) == Complete(Time.zero),
        const("rhino") |> valueAt(Time.zero) == "rhino",
      ],
    )
  });

  describe("animate", ({test, _}) => {
    assertMany(
      "examples",
      test,
      Animation.[
        (animate(Time.seconds(5.)) |> valueAt(Time.zero) :> float) == 0.0,
        (animate(Time.seconds(5.)) |> valueAt(Time.seconds(-42.)) :> float)
        == 0.0,
        (animate(Time.seconds(5.)) |> valueAt(Time.seconds(1.)) :> float)
        == 0.2,
        (animate(Time.seconds(5.)) |> valueAt(Time.seconds(42.)) :> float)
        == 1.0,
        animate(Time.seconds(5.)) |> stateAt(Time.zero) == Running,
        animate(Time.seconds(5.)) |> stateAt(Time.seconds(-42.)) == Delayed,
        animate(Time.seconds(5.)) |> stateAt(Time.seconds(1.)) == Running,
        animate(Time.seconds(5.))
        |> stateAt(Time.seconds(42.)) == Complete(Time.seconds(5.)),
      ],
    )
  });

  describe("delay", ({test, _}) => {
    test("basics", ({expect, _}) => {
      open Animation;

      let anim =
        animate(Time.seconds(1.)) |> delay(Time.seconds(1.)) |> floatify;

      expect.float(anim |> valueAt(Time.seconds(-2.))).toBeCloseTo(0.);
      expect.float(anim |> valueAt(Time.seconds(0.75))).toBeCloseTo(0.);
      expect.float(anim |> valueAt(Time.seconds(1.5))).toBeCloseTo(0.5);
      expect.float(anim |> valueAt(Time.seconds(4.66))).toBeCloseTo(1.);
    });

    assertMany(
      "examples",
      test,
      Animation.[
        (
          animate(Time.seconds(1.))
          |> delay(Time.seconds(1.))
          |> valueAt(Time.seconds(-2.)) :> float
        )
        == 0.0,
        (
          animate(Time.seconds(1.))
          |> delay(Time.seconds(1.))
          |> valueAt(Time.seconds(0.75)) :> float
        )
        == 0.0,
        (
          animate(Time.seconds(1.))
          |> delay(Time.seconds(1.))
          |> valueAt(Time.seconds(1.5)) :> float
        )
        == 0.5,
        (
          animate(Time.seconds(1.))
          |> delay(Time.seconds(1.))
          |> valueAt(Time.seconds(4.66)) :> float
        )
        == 1.0,
        animate(Time.seconds(1.))
        |> delay(Time.seconds(1.))
        |> stateAt(Time.seconds(-2.)) == Delayed,
        animate(Time.seconds(1.))
        |> delay(Time.seconds(1.))
        |> stateAt(Time.seconds(0.75)) == Delayed,
        animate(Time.seconds(1.))
        |> delay(Time.seconds(1.))
        |> stateAt(Time.seconds(1.5)) == Running,
        animate(Time.seconds(1.))
        |> delay(Time.seconds(1.))
        |> stateAt(Time.seconds(4.66)) == Complete(Time.seconds(2.)),
      ],
    );

    test("should accumulate", ({expect, _}) => {
      open Animation;

      let anim =
        animate(Time.seconds(1.))
        |> delay(Time.seconds(1.2))
        |> delay(Time.seconds(2.3))
        |> floatify;

      Animation.(
        {
          expect.ext.animationState(stateAt(Time.seconds(3.49), anim)).
            toEqual(
            Delayed,
          );
          expect.ext.animationState(stateAt(Time.seconds(3.5), anim)).
            toEqual(
            Running,
          );
          expect.ext.animationState(stateAt(Time.seconds(4.), anim)).toEqual(
            Running,
          );
          expect.ext.animationState(stateAt(Time.seconds(5.), anim)).toEqual(
            Complete(Time.seconds(4.5)),
          );
        }
      );
    });
  });

  describe("repeat", ({test, _}) => {
    test("basics", ({expect, _}) => {
      open Animation;

      let anim = animate(Time.seconds(1.)) |> repeat |> floatify;

      expect.float(anim |> valueAt(Time.seconds(-2.))).toBeCloseTo(0.);
      expect.float(anim |> valueAt(Time.seconds(0.75))).toBeCloseTo(0.75);
      expect.float(anim |> valueAt(Time.seconds(1.75))).toBeCloseTo(0.75);
      expect.float(anim |> valueAt(Time.seconds(3.66))).toBeCloseTo(0.66);
      expect.ext.animationState(anim |> stateAt(Time.seconds(3.66))).toEqual(
        Running,
      );
      expect.float(anim |> valueAt(Time.seconds(4.66))).toBeCloseTo(0.66);
    });

    assertMany(
      "examples",
      test,
      Animation.[
        (
          animate(Time.seconds(1.)) |> repeat |> valueAt(Time.seconds(-2.)) :> float
        )
        == 0.0,
        (
          animate(Time.seconds(1.)) |> repeat |> valueAt(Time.seconds(0.75)) :> float
        )
        == 0.75,
        (
          animate(Time.seconds(1.)) |> repeat |> valueAt(Time.seconds(1.75)) :> float
        )
        == 0.75,
        (
          animate(Time.seconds(1.)) |> repeat |> valueAt(Time.seconds(3.66)) :> float
        )
        =~. 0.66,
        (
          animate(Time.seconds(1.)) |> repeat |> valueAt(Time.seconds(4.66)) :> float
        )
        =~. 0.66,
        animate(Time.seconds(1.))
        |> repeat
        |> stateAt(Time.seconds(-2.)) == Delayed,
        animate(Time.seconds(1.))
        |> repeat
        |> stateAt(Time.seconds(4.66)) == Running,
      ],
    );
  });

  describe("alternatingRepeat", ({test, _}) => {
    test("nbasics", ({expect, _}) => {
      open Animation;

      let anim = animate(Time.seconds(1.)) |> alternatingRepeat |> floatify;

      expect.float(anim |> valueAt(Time.seconds(-2.))).toBeCloseTo(0.);
      expect.float(anim |> valueAt(Time.seconds(0.75))).toBeCloseTo(0.75);
      expect.float(anim |> valueAt(Time.seconds(1.75))).toBeCloseTo(0.25);
      expect.float(anim |> valueAt(Time.seconds(3.66))).toBeCloseTo(0.34);
      expect.float(anim |> valueAt(Time.seconds(4.66))).toBeCloseTo(0.66);
    });

    assertMany(
      "examples",
      test,
      Animation.[
        (
          animate(Time.seconds(1.))
          |> alternatingRepeat
          |> valueAt(Time.seconds(-2.)) :> float
        )
        == 0.0,
        (
          animate(Time.seconds(1.))
          |> alternatingRepeat
          |> valueAt(Time.seconds(0.75)) :> float
        )
        == 0.75,
        (
          animate(Time.seconds(1.))
          |> alternatingRepeat
          |> valueAt(Time.seconds(1.75)) :> float
        )
        == 0.25,
        (
          animate(Time.seconds(1.))
          |> alternatingRepeat
          |> valueAt(Time.seconds(3.66)) :> float
        )
        =~. 0.34,
        (
          animate(Time.seconds(1.))
          |> alternatingRepeat
          |> valueAt(Time.seconds(4.66)) :> float
        )
        =~. 0.66,
        animate(Time.seconds(1.))
        |> alternatingRepeat
        |> stateAt(Time.seconds(-2.)) == Delayed,
        animate(Time.seconds(1.))
        |> alternatingRepeat
        |> stateAt(Time.seconds(4.66)) == Running,
      ],
    );
  });

  describe("ease", ({test, _}) => {
    test("basics", ({expect, _}) => {
      open Animation;

      let anim =
        animate(Time.seconds(1.)) |> ease(Easing.quadratic) |> floatify;

      expect.float(anim |> valueAt(Time.seconds(-2.))).toBeCloseTo(0.);
      expect.float(anim |> valueAt(Time.seconds(0.25))).toBeCloseTo(
        0.0625,
      );
      expect.float(anim |> valueAt(Time.seconds(0.5))).toBeCloseTo(0.25);
      expect.float(anim |> valueAt(Time.seconds(0.75))).toBeCloseTo(
        0.5625,
      );
      expect.float(anim |> valueAt(Time.seconds(2.))).toBeCloseTo(1.);
    });

    assertMany(
      "examples",
      test,
      Animation.[
        (
          animate(Time.seconds(1.))
          |> ease(Easing.quadratic)
          |> valueAt(Time.seconds(-2.)) :> float
        )
        == 0.0,
        (
          animate(Time.seconds(1.))
          |> ease(Easing.quadratic)
          |> valueAt(Time.seconds(0.25)) :> float
        )
        == 0.0625,
        (
          animate(Time.seconds(1.))
          |> ease(Easing.quadratic)
          |> valueAt(Time.seconds(0.5)) :> float
        )
        == 0.25,
        (
          animate(Time.seconds(1.))
          |> ease(Easing.quadratic)
          |> valueAt(Time.seconds(0.75)) :> float
        )
        =~. 0.5625,
        (
          animate(Time.seconds(1.))
          |> ease(Easing.quadratic)
          |> valueAt(Time.seconds(4.66)) :> float
        )
        =~. 1.0,
      ],
    );
  });

  describe("ease", ({test, _}) => {
    test("basics", ({expect, _}) => {
      open Animation;

      let anim = animate(Time.seconds(1.)) |> tween(100., 110.);

      expect.float(anim |> valueAt(Time.seconds(-2.))).toBeCloseTo(100.);
      expect.float(anim |> valueAt(Time.seconds(0.25))).toBeCloseTo(102.5);
      expect.float(anim |> valueAt(Time.seconds(0.5))).toBeCloseTo(105.);
      expect.float(anim |> valueAt(Time.seconds(0.75))).toBeCloseTo(107.5);
      expect.float(anim |> valueAt(Time.seconds(2.))).toBeCloseTo(110.);
    });

    assertMany(
      "examples",
      test,
      Animation.[
        animate(Time.seconds(1.))
        |> tween(2., 5.)
        |> valueAt(Time.zero) == 2.0,
        animate(Time.seconds(1.))
        |> tween(2., 5.)
        |> valueAt(Time.seconds(0.5)) == 3.5,
        animate(Time.seconds(1.))
        |> tween(2., 5.)
        |> valueAt(Time.seconds(-42.)) == 2.0,
        animate(Time.seconds(1.))
        |> tween(2., 5.)
        |> valueAt(Time.seconds(42.)) == 5.0,
      ],
    );
  });

  describe("map", ({test, _}) => {
    test("basics", ({expect, _}) => {
      open Animation;

      let anim =
        animate(Time.seconds(1.)) |> floatify |> map(string_of_float);

      expect.string(anim |> valueAt(Time.seconds(-2.))).toEqual("0.");
      expect.string(anim |> valueAt(Time.seconds(0.75))).toEqual("0.75");
      expect.string(anim |> valueAt(Time.seconds(1.5))).toEqual("1.");
    });

    assertMany(
      "examples",
      test,
      Animation.[
        animate(Time.seconds(1.))
        |> map(n => string_of_float((n: NormalizedTime.t :> float)))
        |> valueAt(Time.seconds(0.33)) == "0.33",
      ],
    );
  });

  describe("andThen", ({test, _}) => {
    test("basics", ({expect, _}) => {
      open Animation;

      let first = animate(Time.seconds(1.)) |> tween(0., 10.);
      let second = animate(Time.seconds(1.)) |> tween(30., 20.);
      let anim = andThen(first, ~next=second);

      expect.float(anim |> valueAt(Time.seconds(-2.))).toBeCloseTo(0.);
      expect.float(anim |> valueAt(Time.seconds(0.75))).toBeCloseTo(7.5);
      expect.float(anim |> valueAt(Time.seconds(1.5))).toBeCloseTo(25.);
      expect.float(anim |> valueAt(Time.seconds(4.66))).toBeCloseTo(20.);
    });

    assertMany(
      "examples",
      test,
      Animation.[
        (
          animate(Time.seconds(1.))
          |> andThen(~next=animate(Time.seconds(2.)))
          |> valueAt(Time.seconds(1.)) :> float
        )
        == 1.0,
        (
          animate(Time.seconds(1.))
          |> andThen(~next=animate(Time.seconds(2.)))
          |> valueAt(Time.seconds(2.)) :> float
        )
        == 0.5,
      ],
    );
  });

  describe("zip", ({test, _}) => {
    test("basics", ({expect, _}) => {
      open Animation;

      let first = animate(Time.seconds(1.)) |> tween(0., 10.);
      let second = animate(Time.seconds(1.)) |> tween(30., 20.);
      let anim = zip((first, second));

      expect.float(anim |> valueAt(Time.seconds(-2.)) |> fst).toBeCloseTo(
        0.,
      );
      expect.float(anim |> valueAt(Time.seconds(-2.)) |> snd).toBeCloseTo(
        30.,
      );
      expect.float(anim |> valueAt(Time.seconds(0.75)) |> fst).toBeCloseTo(
        7.5,
      );
      expect.float(anim |> valueAt(Time.seconds(0.75)) |> snd).toBeCloseTo(
        22.5,
      );
      expect.float(anim |> valueAt(Time.seconds(1.5)) |> fst).toBeCloseTo(
        10.,
      );
      expect.float(anim |> valueAt(Time.seconds(1.5)) |> snd).toBeCloseTo(
        20.,
      );
      expect.float(anim |> valueAt(Time.seconds(4.66)) |> fst).toBeCloseTo(
        10.,
      );
      expect.float(anim |> valueAt(Time.seconds(4.66)) |> snd).toBeCloseTo(
        20.,
      );
    });

    assertMany(
      "examples",
      test,
      Animation.[
        (
          zip((animate(Time.seconds(1.)), animate(Time.seconds(2.))))
          |> valueAt(Time.seconds(1.))
          |> fst :> float
        )
        == 1.0,
        (
          zip((animate(Time.seconds(1.)), animate(Time.seconds(2.))))
          |> valueAt(Time.seconds(1.))
          |> snd :> float
        )
        == 0.5,
      ],
    );
  });

  describe("apply", ({test, _}) => {
    assertMany(
      "examples",
      test,
      Animation.[
        (
          animate(Time.seconds(1.)) |> apply(Time.seconds(0.33)) |> fst :> float
        )
        == 0.33,
        animate(Time.seconds(1.))
        |> apply(Time.seconds(0.33))
        |> snd == Running,
      ],
    )
  });

  describe("valueAt", ({test, _}) => {
    assertMany(
      "examples",
      test,
      Animation.[
        (animate(Time.seconds(1.)) |> valueAt(Time.seconds(0.33)) :> float)
        == 0.33,
      ],
    )
  });

  describe("stateAt", ({test, _}) => {
    assertMany(
      "examples",
      test,
      Animation.[
        animate(Time.seconds(1.)) |> stateAt(Time.seconds(0.33)) == Running,
      ],
    )
  });
});