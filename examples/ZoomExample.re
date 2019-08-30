open Revery;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

let zoomButton = (~zoom, ~onClick, ~children as _, ()) => {
      <Button
        height=50
        width=100
        fontSize=15
        title=string_of_float(zoom)
        onClick={() => onClick(zoom)}
      />
};

module Zoom = {
  let component = React.component("Zoom");

  let createElement = (~children as _, ()) =>
    component(hooks => {

      let window = UI.getActiveWindow();
      let zoomV = switch (window) {
      | Some(v) => Window.getZoom(v)
      | None => -1.0;
      };

    let (currentZoom, setCurrentZoom, hooks) = Hooks.state(zoomV, hooks);

      let textStyle =
        Style.[
          color(Colors.white),
          width(100),
          fontFamily("Roboto-Regular.ttf"),
          fontSize(16),
          margin(14),
        ];

      let setZoom = (zoom) => {
        switch (window) {
        | Some(v) => 
            Window.setZoom(v, zoom);
            setCurrentZoom(zoom);
        | None => ()
        };
      };

        print_endline ("Zoomv: " ++ string_of_float(currentZoom));
      
        (hooks,
        <Center>
          <Column>
            <Text style=textStyle text={"Zoom: " ++ string_of_float(currentZoom)} />
            <Row>
            <zoomButton zoom=0.5 onClick=setZoom />
            <zoomButton zoom=1.0 onClick=setZoom />
            <zoomButton zoom=1.25 onClick=setZoom />
            <zoomButton zoom=1.5 onClick=setZoom />
            <zoomButton zoom=2.0 onClick=setZoom />
            </Row>
          </Column>
        </Center>
        );
});

};

let render = () => <Zoom />;
