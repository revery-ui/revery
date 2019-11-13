open Revery_Core;
open Revery_UI;

open Rely.MatcherTypes;

// Approximate equality for floats
let (=~.) = (~tolerance=0.0001, a, b) => abs_float(a -. b) < tolerance;

type animationStateExtensions = {toEqual: Animation.state => unit};

let animationStateExtensions = (actual, {createMatcher}) => {
  let pass = (() => "", true);
  let fail = message => (() => message, false);

  let toString =
    Animation.(
      fun
      | Delayed => "Delayed"
      | Running => "Running"
      | Complete(elapsed) =>
        Printf.sprintf("Complete(%s)", Time.toString(elapsed))
    );

  let createStateMatcher =
    createMatcher(
      ({formatReceived, formatExpected, _}, actualThunk, expectedThunk) => {
      let actual = actualThunk();
      let expected = expectedThunk();

      Animation.(
        switch (actual, expected) {
        | (Delayed, Delayed)
        | (Running, Running) => pass
        | (Complete(a), Complete(b))
            when Time.toFloatSeconds(a) =~. Time.toFloatSeconds(b) => pass
        | _ =>
          let failureMessage =
            Printf.sprintf(
              "Expected: %s\nReceived: %s",
              formatExpected(toString(expected)),
              formatReceived(toString(actual)),
            );
          fail(failureMessage);
        }
      );
    });

  {toEqual: expected => createStateMatcher(() => actual, () => expected)};
};

type customMatchers = {
  animationState: Animation.state => animationStateExtensions,
};

let customMatchers = createMatcher => {
  animationState: actual => animationStateExtensions(actual, createMatcher),
};

include Rely.Make({
  let config =
    Rely.TestFrameworkConfig.initialize({
      snapshotDir: "test/UI/__snapshots__",
      projectDir: "",
    });
});

let {describe, describeOnly, describeSkip} =
  describeConfig |> withCustomMatchers(customMatchers) |> build;

let assertOne = (label, test, assertion) =>
  test(label, ({expect, _}) =>
    expect.bool(assertion).toBeTrue()
  );

let assertMany = (label, test) =>
  List.iteri((i, assertion) =>
    assertOne(Printf.sprintf("%s %n", label, i), test, assertion)
  );
