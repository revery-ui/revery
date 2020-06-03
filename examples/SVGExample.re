open Revery;
open Revery.UI;
open Revery.UI.Components;

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

let example2 =
  {|
    <svg viewBox="0 0 200 200">
      <g id="grid">
        <!-- Vertical -->
        <line x1="50" y1="0" x2="50" y2="500" stroke="#888" />
        <line x1="100" y1="0" x2="100" y2="500" stroke="#888" />
        <line x1="150" y1="0" x2="150" y2="500" stroke="#888" />
        <line x1="200" y1="0" x2="200" y2="500" stroke="#888" />
        <line x1="250" y1="0" x2="250" y2="500" stroke="#888" />
        <line x1="300" y1="0" x2="300" y2="500" stroke="#888" />
        <line x1="350" y1="0" x2="350" y2="500" stroke="#888" />
        <line x1="400" y1="0" x2="400" y2="500" stroke="#888" />
        <line x1="450" y1="0" x2="450" y2="500" stroke="#888" />

        <!-- Horizontal -->
        <line x1="0" y1="50" x2="500" y2="50" stroke="#555" />
        <line x1="0" y1="100" x2="500" y2="100" stroke="#555" />
        <line x1="0" y1="150" x2="500" y2="150" stroke="#555" />
        <line x1="0" y1="200" x2="500" y2="200" stroke="#555" />
        <line x1="0" y1="250" x2="500" y2="250" stroke="#555" />
        <line x1="0" y1="300" x2="500" y2="300" stroke="#555" />
        <line x1="0" y1="350" x2="500" y2="350" stroke="#555" />
        <line x1="0" y1="400" x2="500" y2="400" stroke="#555" />
        <line x1="0" y1="450" x2="500" y2="450" stroke="#555" />
      </g>
      <g>
        <rect x="10" y="10" rx="10" ry="5" width="50" height="50" fill="#00f" stroke="#fff" />
        <x circle cx="10" cy="10" r="30" fill="#f00" />
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
    <SVGString style=outerBox contents=example2 />
  </View>;
