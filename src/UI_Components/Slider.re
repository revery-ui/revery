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

type valueChangedFunction = (float) => unit;
let noopValueChanged = (_f) => ();

type setStateFunction = int => unit;

let latestState: ref(setStateFunction) = ref((_) => ());

/* The 'include' here makes the component available at the top level */
include (
          val component((render, ~_style=Style.make(()), ~onValueChanged:valueChangedFunction=noopValueChanged, ~_minimumValue=0., ~_maximumValue=1., ~children, ()) =>
                render(
                  () => {
                    let (ref, _setRef) = useState(None);
                    let (curOffset, _setOffset) = useState(0);

                    latestState := _setOffset;

                    let setRef = (r) => {
                        print_endline ("SETTING REF: " ++ string_of_int(r#getInternalId()));
                        _setRef(Some(r));
                    };

                    let onMouseDown = (_evt) => {
                               print_endline ("MOUSEDOWN");
                       Mouse.setCapture(
                           ~onMouseMove=(evt) => { 
                               print_endline ("MOVING: " ++ string_of_float(evt.mouseX));
                               (latestState^)(int_of_float(evt.mouseX)); 

                               switch(ref) {
                               | Some(r) => print_endline("got ref: " ++ string_of_int(r#getInternalId()));
                               | None => print_endline("no ref");
                               };

                               /* TODO: Proper calculation */
                               onValueChanged(evt.mouseX);


                           },
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

                    <view onMouseDown style ref={(r) => setRef(r)}> 
                        <view style=Style.make(~position=LayoutTypes.Absolute, ~height=25, ~width=15, ~left=curOffset, ~top=0, ~backgroundColor=Colors.yellow, ()) />
                    </view>;
                  },
                  ~children,
                )
              )
        );
