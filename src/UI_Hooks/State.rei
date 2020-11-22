open Revery_UI.React.Hooks;

/**
{2 Description:}

TODO

{2 Usage:}

{[
let%hook (state, setState) = Hooks.state(0);
]}
*/
let state:
  ('a, t(State.t('a) => 'b, 'c)) => (('a, ('a => 'a) => unit), t('b, 'c));
