open Revery_UI;
open Revery_Core;

let component = React.component("Positioned");

let style = Style.[position(`Relative)];

let createElement = (~top=?, ~left=?, ~right=?, ~bottom=?, ~children, ()) =>
  component(hooks => {
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
      | _ => Style.[]
      };

    (hooks, <View style> ...children </View>);
  });
