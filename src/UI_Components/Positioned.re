open Revery_UI;
open Revery_UI_Primitives;

module X = Revery_UI.Brisk_jsx_runtime;

[@component]
let make = (~top=?, ~left=?, ~right=?, ~bottom=?, ~children, (), hooks) => {
  let style =
    switch (top, left, right, bottom) {
    | (Some(t), Some(l), _, _) =>
      Style.[position(`Absolute), top(t), left(l)]
    | (Some(t), _, Some(r), _) =>
      Style.[position(`Absolute), top(t), right(r)]
    | (_, _, Some(r), Some(b)) =>
      Style.[position(`Absolute), bottom(b), right(r)]
    | (_, Some(l), _, Some(b)) =>
      Style.[position(`Absolute), bottom(b), left(l)]
    | _ => []
    };

  (hooks, <View style> ...children </View>);
};
