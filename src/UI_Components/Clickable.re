/*
 * Clickable.re
 *
 * This module provides a `<Clickable />` component, loosely inspired by the
 * `<Touchable />` component in React-Native, but geared towards the desktop.
 */

open Revery_UI;
open Revery_Core;

type clickFunction = unit => unit;
let noop = () => ();

/* The 'include' here makes the component available at the top level */
include (
          val component((render, ~onClick: clickFunction=noop, ~children, ()) =>
                render(
                  () => {
                    let (opacity, setOpacity) = useState(0.8);

                    /* TODO:
                     *
                     * This logic isn't really correct,
                     * for the case where you hold down the mouse,
                     * move around (leave the item), and come back.
                     */
                    let onMouseDown = _ => setOpacity(1.0);
                    let onMouseUp = _ => {
                      setOpacity(0.8);
                      onClick();
                    };

                    let style =
                      Style.make(~opacity, ~cursor=MouseCursors.pointer, ());

                    <view style onMouseDown onMouseUp> ...children </view>;
                  },
                  ~children,
                )
              )
        );
