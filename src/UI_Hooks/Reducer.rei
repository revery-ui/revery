open Revery_UI.React.Hooks;

/**
{2 Description:}

An alternative to Hooks.state. Accepts a reducer of type (action, state) => newState, and returns the current state paired with a dispatch function.

{2 Usage:}

{[
let%hook (state, dispatch) = Hooks.reducer(~initialState={counter:0}, reduce);
]}
*/
let reducer:
  (~initialState: 'a, ('b, 'a) => 'a, t(Reducer.t('a) => 'c, 'd)) =>
  (('a, 'b => unit), t('c, 'd));
