/*
 * Clickable.re
 *
 * This module provides a `<Clickable />` component, loosely inspired by the
 * `<Touchable />` component in React-Native, but geared towards the desktop.
 */

open Revery_UI;

type clickFunction = unit => unit;
let noop = () => ();

module Clickable = (val component((render, ~onClick:clickFunction=noop, ~children, ()) =>
    render(() => {

        let (opacity, setOpacity) = useState(0.8);

        let onMouseDown = _ => setOpacity(1.0);
        let onMouseUp = _ => {
            setOpacity(0.8);
            onClick();
        };

        let style = Style.make(~opacity, ());

        <view style onMouseDown onMouseUp>
            ...children
        </view>
        
    },~children)));
