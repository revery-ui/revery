open Revery;
open Revery.UI;
open Revery.UI.Components;

let examples = [
  (
    "Kitchen Sink",
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
          <circle cx="10" cy="10" r="30" fill="#f00" />
          <ellipse cx="100" cy="50" rx="100" ry="50" fill="#fff" />
        </g>
        <g>
          <path fill="none" stroke="#ff0"
            d="M 10,10 L 10 40 M 10 10 L 40,10" />
          <path stroke="#f0f"
            d="M 50,50 l 0 30 M 50 50 l 30,0" />
          <path stroke="#0ff"
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
        <g id="arc">
          <path fill="none" stroke="#f00"
                d="M 6,10
                  A 6 4 10 1 0 14,10" />

          <path fill="none" stroke="#0f0"
                d="M 6,10
                  A 6 4 10 1 1 14,10" />

          <path fill="none" stroke="#f0f"
                d="M 6,10
                  A 6 4 10 0 1 14,10" />

          <path fill="none" stroke="#faa"
                d="M 6,10
                  A 6 4 10 0 0 14,10" />
        </g>
        <g id="polygon">
          <polygon points="0,100 50,25 50,75 100,0" fill="lime"/>

          <polygon points="100,100 150,25 150,75 200,0"
                    fill="none" stroke="white" />
        </g>
      </svg>
    |},
  ),
  (
    "<polygon>",
    {|
      <!-- Source: https://developer.mozilla.org/en-US/docs/Web/SVG/Element/polygon -->

      <svg viewBox="0 0 200 100" xmlns="http://www.w3.org/2000/svg">
        <!-- Example of a polygon with the default fill -->
        <polygon points="0,100 50,25 50,75 100,0" />

        <!-- Example of the same polygon shape with stroke and no fill -->
        <polygon points="100,100 150,25 150,75 200,0"
                  fill="none" stroke="black" />
      </svg>
    |},
  ),
  (
    "<polyline>",
    {|
      <!-- Source: https://developer.mozilla.org/en-US/docs/Web/SVG/Element/polyline -->

      <svg viewBox="0 0 200 100" xmlns="http://www.w3.org/2000/svg">
        <!-- Example of a polyline with the default fill -->
        <polyline points="0,100 50,25 50,75 100,0" />

        <!-- Example of the same polyline shape with stroke and no fill -->
        <polyline points="100,100 150,25 150,75 200,0"
                  fill="none" stroke="black" />
      </svg>
    |},
  ),
  (
    "<rect>",
    {|
      <!-- Source: https://developer.mozilla.org/en-US/docs/Web/SVG/Element/rect -->

      <svg viewBox="0 0 220 100" xmlns="http://www.w3.org/2000/svg">
        <!-- Simple rectangle -->
        <rect width="100" height="100" />

        <!-- Rounded corner rectangle -->
        <rect x="120" width="100" height="100" rx="15" />
      </svg>
    |},
  ),
  (
    "<path> - heart",
    {|
      <!-- Source: https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/d -->

      <svg viewBox="0 0 100 100" xmlns="http://www.w3.org/2000/svg">
        <path fill="none" stroke="red"
          d="M 10,30
            A 20,20 0,0,1 50,30
            A 20,20 0,0,1 90,30
            Q 90,60 50,90
            Q 10,60 10,30 z" />
      </svg>
    |},
  ),
  (
    "<path> - moveTo",
    {|
      <!-- Source: https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/d#Path_commands -->

      <svg viewBox="0 0 100 100" xmlns="http://www.w3.org/2000/svg">
        <path fill="none" stroke="red"
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
      </svg>
    |},
  ),
  (
    "<path> - lineTo",
    {|
      <!-- Source: https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/d#Path_commands -->

      <svg viewBox="0 0 200 100" xmlns="http://www.w3.org/2000/svg">
        <!-- LineTo commands with absolute coordinates -->
        <path fill="none" stroke="red"
              d="M 10,10
                L 90,90
                V 10
                H 50" />

        <!-- LineTo commands with relative coordinates -->
        <path fill="none" stroke="red"
              d="M 110,10
                l 80,80
                v -80
                h -40" />
      </svg>
    |},
  ),
  (
    "<path> - lineTo*",
    {|
      <svg viewBox="0 0 200 100" xmlns="http://www.w3.org/2000/svg">
        <!-- LineTo commands with absolute coordinates -->
        <path fill="none" stroke="red"
              d="M 10,10
                L 90,90
                L 90,10
                L 50,10" />

        <!-- LineTo commands with relative coordinates -->
        <path fill="none" stroke="red"
              d="M 110,10
                l 80,80
                v -80
                h -40" />
      </svg>
    |},
  ),
  (
    "<path> - cubic bezier",
    {|
      <!-- Source: https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/d#Path_commands -->

      <svg viewBox="0 0 200 100" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">

        <!-- Cubic Bézier curve with absolute coordinates -->
        <path fill="none" stroke="red"
              d="M 10,90
                C 30,90 25,10 50,10
                S 70,90 90,90" />

        <!-- Cubic Bézier curve with relative coordinates -->
        <path fill="none" stroke="red"
              d="M 110,90
                c 20,0 15,-80 40,-80
                s 20,80 40,80" />

        <!-- Highlight the curve vertex and control points -->
        <g id="ControlPoints">

          <!-- First cubic command control points -->
          <line x1="10" y1="90" x2="30" y2="90" stroke="lightgrey" />
          <circle cx="30" cy="90" r="1.5"/>

          <line x1="50" y1="10" x2="25" y2="10" stroke="lightgrey" />
          <circle cx="25" cy="10" r="1.5"/>

          <!-- Second smooth command control points (the first one is implicit) -->
          <line x1="50" y1="10" x2="75" y2="10" stroke="lightgrey" stroke-dasharray="2" />
          <circle cx="75" cy="10" r="1.5" fill="lightgrey"/>

          <line x1="90" y1="90" x2="70" y2="90" stroke="lightgrey" />
          <circle cx="70" cy="90" r="1.5" />

          <!-- curve vertex points -->
          <circle cx="10" cy="90" r="1.5"/>
          <circle cx="50" cy="10" r="1.5"/>
          <circle cx="90" cy="90" r="1.5"/>
        </g>
        <use xlink:href="#ControlPoints" x="100" />
      </svg>
    |},
  ),
  (
    "<path> - cubic bezier*",
    {|
      <svg viewBox="0 0 200 100" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">

        <!-- Cubic Bézier curve with absolute coordinates -->
        <path fill="none" stroke="red"
              d="M 10,90
                C 30,90 25,10 50,10
                C 75,10 70,90 90,90" />

        <!-- Cubic Bézier curve with relative coordinates -->
        <path fill="none" stroke="red"
              d="M 110,90
                c 20,0 15,-80 40,-80
                c 20,0 20,80 40,80" />

        <!-- Highlight the curve vertex and control points -->
        <g id="ControlPoints">

          <!-- First cubic command control points -->
          <line x1="10" y1="90" x2="30" y2="90" stroke="lightgrey" />
          <circle cx="30" cy="90" r="1.5"/>

          <line x1="50" y1="10" x2="25" y2="10" stroke="lightgrey" />
          <circle cx="25" cy="10" r="1.5"/>

          <!-- Second smooth command control points (the first one is implicit) -->
          <line x1="50" y1="10" x2="75" y2="10" stroke="lightgrey" stroke-dasharray="2" />
          <circle cx="75" cy="10" r="1.5" fill="lightgrey"/>

          <line x1="90" y1="90" x2="70" y2="90" stroke="lightgrey" />
          <circle cx="70" cy="90" r="1.5" />

          <!-- curve vertex points -->
          <circle cx="10" cy="90" r="1.5"/>
          <circle cx="50" cy="10" r="1.5"/>
          <circle cx="90" cy="90" r="1.5"/>
        </g>
        <use xlink:href="#ControlPoints" x="100" />
      </svg>
    |},
  ),
  (
    "<path> - quadratic bezier",
    {|
      <!-- Source: https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/d#Path_commands -->

      <svg viewBox="0 0 200 100" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">

        <!-- Quadratic Bézier curve with implicit repetition -->
        <path fill="none" stroke="red"
              d="M 10,50
                Q 25,25 40,50
                t 30,0 30,0 30,0 30,0 30,0" />

        <!-- Highlight the curve vertex and control points -->
        <g>
          <polyline points="10,50 25,25 40,50" stroke="rgba(0,0,0,.2)" fill="none" />
          <circle cx="25" cy="25" r="1.5" />

          <!-- Curve vertex points -->
          <circle cx="10" cy="50" r="1.5"/>
          <circle cx="40" cy="50" r="1.5"/>

          <g id="SmoothQuadraticDown">
            <polyline points="40,50 55,75 70,50" stroke="rgba(0,0,0,.2)" stroke-dasharray="2" fill="none" />
            <circle cx="55" cy="75" r="1.5" fill="lightgrey" />
            <circle cx="70" cy="50" r="1.5" />
          </g>

          <g id="SmoothQuadraticUp">
            <polyline points="70,50 85,25 100,50" stroke="rgba(0,0,0,.2)" stroke-dasharray="2" fill="none" />
            <circle cx="85" cy="25" r="1.5" fill="lightgrey" />
            <circle cx="100" cy="50" r="1.5" />
          </g>

          <use xlink:href="#SmoothQuadraticDown" x="60" />
          <use xlink:href="#SmoothQuadraticUp"   x="60" />
          <use xlink:href="#SmoothQuadraticDown" x="120" />
        </g>
      </svg>
    |},
  ),
  (
    "<path> - quadratic bezier*",
    {|
      <svg viewBox="0 0 200 100" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">

        <!-- Quadratic Bézier curve with implicit repetition -->
        <path fill="none" stroke="red"
              d="M 10,50
                Q 25,25 40,50
                q 15,25 30,0
                q 15,-25 30,0
                q 15,25 30,0
                q 15,-25 30,0
                q 15,25 30,0
                q 15,-25 30,0" />

        <!-- Highlight the curve vertex and control points -->
        <g>
          <polyline points="10,50 25,25 40,50" stroke="#0003" fill="none" />
          <circle cx="25" cy="25" r="1.5" />

          <!-- Curve vertex points -->
          <circle cx="10" cy="50" r="1.5"/>
          <circle cx="40" cy="50" r="1.5"/>

          <g id="SmoothQuadraticDown">
            <polyline points="40,50 55,75 70,50" stroke="#0003" stroke-dasharray="2" fill="none" />
            <circle cx="55" cy="75" r="1.5" fill="lightgrey" />
            <circle cx="70" cy="50" r="1.5" />
          </g>

          <g id="SmoothQuadraticUp">
            <polyline points="70,50 85,25 100,50" stroke="#0003" stroke-dasharray="2" fill="none" />
            <circle cx="85" cy="25" r="1.5" fill="lightgrey" />
            <circle cx="100" cy="50" r="1.5" />
          </g>

          <use xlink:href="#SmoothQuadraticDown" x="60" />
          <use xlink:href="#SmoothQuadraticUp"   x="60" />
          <use xlink:href="#SmoothQuadraticDown" x="120" />
        </g>
      </svg>
    |},
  ),
  (
    "<path> - elliptic arc",
    {|
      <!-- Source: https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/d#Path_commands -->

      <svg viewBox="0 0 20 20" xmlns="http://www.w3.org/2000/svg">

        <!-- The influence of the arc flags with which the arc is drawn -->
        <path fill="none" stroke="red"
              d="M 6,10
                A 6 4 10 1 0 14,10" />

        <path fill="none" stroke="lime"
              d="M 6,10
                A 6 4 10 1 1 14,10" />

        <path fill="none" stroke="purple"
              d="M 6,10
                A 6 4 10 0 1 14,10" />

        <path fill="none" stroke="pink"
              d="M 6,10
                A 6 4 10 0 0 14,10" />
      </svg>
    |},
  ),
  (
    "<path> - close path",
    {|
      <!-- Source: https://developer.mozilla.org/en-US/docs/Web/SVG/Attribute/d#Path_commands -->

      <svg viewBox="0 -1 30 11" xmlns="http://www.w3.org/2000/svg">

        <!--
        An open shape with the last point of
        the path different to the first one
        -->
        <path stroke="red"
              d="M 5,1
                l -4,8 8,0" />

        <!--
        An open shape with the last point of
        the path matching the first one
        -->
        <path stroke="red"
              d="M 15,1
                l -4,8 8,0 -4,-8" />

        <!--
        A closed shape with the last point of
        the path different to the first one
        -->
        <path stroke="red"
              d="M 25,1
                l -4,8 8,0
                z" />
      </svg>
    |},
  ),
  (
    "<path> - close path*",
    {|
      <svg viewBox="0 -1 30 11" xmlns="http://www.w3.org/2000/svg">

        <!--
        An open shape with the last point of
        the path different to the first one
        -->
        <path stroke="red"
              d="M 5,1
                l -4,8
                l 8,0" />

        <!--
        An open shape with the last point of
        the path matching the first one
        -->
        <path stroke="red"
              d="M 15,1
                l -4,8
                l 8,0
                l -4,-8" />

        <!--
        A closed shape with the last point of
        the path different to the first one
        -->
        <path stroke="red"
              d="M 25,1
                l -4,8
                l 8,0
                z" />
      </svg>
    |},
  ),
];

module SVGExample = {
  module Styles = {
    open Style;

    let container = [
      backgroundColor(Color.rgba(1., 1., 1., 0.1)),
      flexDirection(`Row),
    ];

    let buttons = [width(200), color(Colors.white), marginLeft(10)];

    let example = [width(350), height(350)];
  };

  let%component make = () => {
    let%hook (currentExample, setExample) =
      Hooks.state(List.hd(examples) |> snd);

    let buttons =
      List.map(
        ((text, value)) => RadioButtonsString.{text, value},
        examples,
      );
    let onChange = data => setExample(_ => data);

    let example =
      switch (currentExample |> SVG.fromString) {
      | Some(svg) => <SVGString style=Styles.example contents=svg />
      | None => <Text text="Parse error: empty string" />
      | exception (Failure(msg)) => <Text text={"Parse error: " ++ msg} />
      };

    <View style=Styles.container>
      <RadioButtonsString
        style=Styles.buttons
        onChange
        defaultSelected=0
        buttons
      />
      example
    </View>;
  };
};

let render = () => <SVGExample />;
