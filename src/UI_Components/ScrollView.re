open Revery_UI;
open Revery_UI.Transform;

include (
          val component(
                (
                  render,
                  ~children,
                  ~style,
                  ~scrollLeft=0,
                  ~scrollTop=0,
                  (),
                ) =>
                render(
                  () => {

                    let innerViewTransform = [
                        TranslateX(-1. *. float_of_int(scrollLeft)),
                        TranslateY(-1. *. float_of_int(scrollTop)),
                    ];
                    
                    <view style>
                      <view style=Style.make(~transform=innerViewTransform, ())>
                        ...children
                      </view>
                    </view>
                  },
                  ~children,
                )
              )
        );
