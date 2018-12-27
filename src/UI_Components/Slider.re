/*
 * Slider.re
 *
 * This module provides a `<Slider />` component, loosely inspired by the
 * `<Slider />` component in React-Native, but geared towards the desktop.
 */

open Revery_Core;
open Revery_UI;

type clickFunction = unit => unit;
let noop = () => ();

/* The 'include' here makes the component available at the top level */
include (
          val component((render, ~_style=Style.make(()), ~_minimumValue=0., ~_maximumValue=1., ~children, ()) =>
                render(
                  () => {
                    let (ref, _setRef) = useState(None);
                    let (curOffset, _setOffset) = useState(0);

                    let setRef = (r) => {
                        print_endline ("SETTING REF");
                        _setRef(r)
                    };

                    let onMouseDown = (_evt) => {
                       Mouse.setCapture(
                           ~onMouseMove=(evt) => { _setOffset(int_of_float(evt.mouseX)); },
                           ~onMouseUp=(_evt) => Mouse.releaseCapture(),
                           ()
                       );
                    }

                    let backgroundColor = switch(ref) {
                    | Some(_) => Colors.red
                    | None => Colors.black
                    };

                    let style = Style.make(
                        ~width=100,
                        ~height=25,
                        ~backgroundColor=backgroundColor,
                        ()
                    );

                    <view onMouseDown style ref={(r) => setRef(Some(r))}> 
                        <view style=Style.make(~position=LayoutTypes.Absolute, ~height=25, ~width=15, ~left=curOffset, ~top=0, ~backgroundColor=Colors.yellow, ()) />
                    </view>;
                  },
                  ~children,
                )
              )
        );
