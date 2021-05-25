open Revery;
open Revery.UI;
open Revery.UI.Components;

module ZoomButton = {
  let make = (~zoom, ~onClick, ()) => {
    <Button
      height=50
      width=100
      fontSize=15.
      title={string_of_float(zoom)}
      onClick={() => onClick(zoom)}
    />;
  };
};

module Zoom = {
  let%component make = () => {
    let window = UI.getActiveWindow();
    let zoomV =
      switch (window) {
      | Some(v) => Window.getZoom(v)
      | None => (-1.0)
      };

    let%hook (currentZoom, setCurrentZoom) = Hooks.state(zoomV);

    let textStyle = Style.[color(Colors.white), width(100), margin(14)];

    let setZoom = zoom => {
      switch (window) {
      | Some(v) =>
        Window.setZoom(v, zoom);
        setCurrentZoom(_ => zoom);
      | None => ()
      };
    };

    print_endline("Zoomv: " ++ string_of_float(currentZoom));

    <Center>
      <Column>
        <Text
          style=textStyle
          fontSize=16.
          text={"Zoom: " ++ string_of_float(currentZoom)}
        />
        <Row>
          <ZoomButton zoom=0.5 onClick=setZoom />
          <ZoomButton zoom=1.0 onClick=setZoom />
          <ZoomButton zoom=1.25 onClick=setZoom />
          <ZoomButton zoom=1.5 onClick=setZoom />
          <ZoomButton zoom=2.0 onClick=setZoom />
        </Row>
      </Column>
    </Center>;
  };
};

let render = () => <Zoom />;
