open Revery;
open Revery.UI;

let containerStyle =
  Style.[
    position(`Absolute),
    top(0),
    bottom(0),
    left(0),
    right(0),
    alignItems(`Center),
    justifyContent(`Center),
    flexDirection(`Column),
  ];

let outerBox =
  Style.[width(450), height(450), backgroundColor(Colors.black)];

let example =
  SVG.Model.{
    defs: [],
    elements: [
      Geometry({
        attributes: [`fill(`color(Colors.red |> Color.toSkia))],
        kind: Circle({cx: `user(10.), cy: `user(10.), r: `user(30.)}),
      }),
      Geometry({
        attributes: [`stroke(`color(Colors.green |> Color.toSkia))],
        kind:
          Line({
            x1: `user(0.),
            y1: `user(0.),
            x2: `user(30.),
            y2: `user(0.),
          }),
      }),
      Geometry({
        attributes: [`stroke(`color(Colors.blue |> Color.toSkia))],
        kind:
          Line({
            x1: `user(0.),
            y1: `user(0.),
            x2: `user(0.),
            y2: `user(30.),
          }),
      }),
    ],
    viewport: {
      origin: {
        x: 10.,
        y: 50.,
      },
      width: 200.,
      height: 100.,
    },
  };

let example2 =
  {|
    <svg>
      <g>
        <rect x="10" y="10" rx="10" ry="5" width="50" height="50" fill="#00f" stroke="#fff" />
        <x circle cx="10" cy="10" r="30" fill="#f00" />
        <x line x1="0" y1="0" x2="30" y2="0" stroke="#0f0" />
        <x line x1="0" y1="0" x2="0" y2="30" stroke="#0f0" />
        <x ellipse cx="100" cy="50" rx="100" ry="50" fill="#fff" />
      </g>
      <g>
        <x path fill="none" stroke="#ff0"
          d="M 10,10 L 10 40 M 10 10 L 40,10" />
        <x path stroke="#f0f"
          d="M 50,50 l 0 30 M 50 50 l 30,0" />
        <x path stroke="#0ff"
          d="M 10,10 h 10
             m  0,10 h 10
             m  0,10 h 10
             M 40,20 h 10
             m  0,10 h 10
             m  0,10 h 10
             m  0,10 h 10
             M 50,50 h 10
             m-20,10 h 10
             m-20,10 h 10
             m-20,10 h 10" />
        <x path stroke="#f00"
              d="M 10,90
                 C 30,90 25,10 50,10
                 S 70,90 90,90" />
        <x path stroke="#f00"
              d="M 110,90
                 c 20,0 15,-80 40,-80
                 s 20,80 40,80" />
        <path stroke="#f00" stroke-width="0.3"
              d="M 10,90
                 C 30,90 25,10 50,10
                 C 75,10 70,90 90,90" />
      </g>
    </svg>
  |}
  |> SVG.fromString
  |> Option.get;

let render = () =>
  <View style=containerStyle>
    <Canvas
      style=outerBox
      render={canvasContext => {
        SVG.render(~width=100., ~height=100., canvasContext, example2)
      }}
    />
  </View>;
