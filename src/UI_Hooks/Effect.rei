open Revery_UI.React.Hooks;

/**
{2 Description:}

The Effect Hook lets you perform side effects in function components.

{2 Usage:}

{[
let%hook () = Hooks.effect(OnMount, () => {
  // Some side-effect

  // Clean-up function
  None;
});
]}
*/
let effect:
  (
    Effect.condition('a),
    unit => option(unit => unit),
    t(Effect.t('a) => 'b, 'c)
  ) =>
  (unit, t('b, 'c));
