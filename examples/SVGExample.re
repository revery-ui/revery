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
    "<path> - quadratic bezier 2",
    {|
     <!-- Source: https://www.w3.org/TR/SVG11/images/paths/quad01.svg -->

      <?xml version="1.0" standalone="no"?>
      <!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN"
        "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
      <svg width="12cm" height="6cm" viewBox="0 0 1200 600"
          xmlns="http://www.w3.org/2000/svg" version="1.1">
        <title>Example quad01 - quadratic Bézier commands in path data</title>
        <desc>Picture showing a "Q" a "T" command,
              along with annotations showing the control points
              and end points</desc>
        <rect x="1" y="1" width="1198" height="598"
              fill="none" stroke="blue" stroke-width="1" />

        <path d="M200,300 Q400,50 600,300 T1000,300"
              fill="none" stroke="red" stroke-width="5"  />
        <!-- End points -->
        <g fill="black" >
          <circle cx="200" cy="300" r="10"/>
          <circle cx="600" cy="300" r="10"/>
          <circle cx="1000" cy="300" r="10"/>
        </g>
        <!-- Control points and lines from end points to control points -->
        <g fill="#888888" >
          <circle cx="400" cy="50" r="10"/>
          <circle cx="800" cy="550" r="10"/>
        </g>
        <path d="M200,300 L400,50 L600,300
                L800,550 L1000,300"
              fill="none" stroke="#888888" stroke-width="2" />
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
    "Material - disc",
    {|
      <!-- Source: https://github.com/PKief/vscode-material-icon-theme/blob/master/icons/disc.svg?short_path=60ae136 -->

      <svg version="1.1" viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg">
        <path d="m12 14.5c-1.3875 0-2.5-1.125-2.5-2.5 0-1.3875 1.1125-2.5 2.5-2.5s2.5 1.1125 2.5 2.5a2.5 2.5 0 0 1-2.5 2.5m0-12.5a10 10 0 0 0-10 10 10 10 0 0 0 10 10 10 10 0 0 0 10-10 10 10 0 0 0-10-10z" style="fill:#b0bec5;stroke-width:1.25"/></svg>
    |},
  ),
  (
    "Material - flash",
    {|
      <!-- Source: https://github.com/PKief/vscode-material-icon-theme/blob/master/icons/flash.svg?short_path=987e419 -->

      <svg version="1.1" viewBox="0 0 24 24" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">
        <defs>
          <linearGradient id="linearGradient4150" x1="2.373" x2="21.86" y1="12.027" y2="12.027" gradientTransform="translate(-.11669 -24.027)" gradientUnits="userSpaceOnUse">
            <stop stop-color="#d92f3c" offset="0"/>
            <stop stop-color="#791223" offset="1"/>
          </linearGradient>
        </defs>
        <g>
          <rect transform="rotate(90)" x="2.2563" y="-21.744" width="19.487" height="19.487" ry="0" fill="url(#linearGradient4150)"/>
          <path d="m16.56 5.9994c-.0046.000383-.0087.00127-.01324.00165-.42659.03629-.83051.09268-1.1813.19192-.55415.166-1.0532.40447-1.494.71803-.4282.32278-.81705.72086-1.1697 1.1912-.34004.47956-.6824 1.0323-1.0224 1.6594-.12109.22868-.23331.42505-.35074.63696v.0017c-1e-4.000183-.0032-.000183-.0033 0l-1.3401 2.394-.005-.0017c-.23782.4425-.46149.84702-.66509 1.1978-.2141.36889-.45189.6815-.71637.93972-.26448.25822-.56659.45561-.90663.59394-.071945.02731-.1614.04195-.24155.06287-.0005144.000134-.00114-.000134-.00165 0h-.9877v2.4138h.98935v-.0017c.42844-.03628.83244-.09229 1.1846-.19192.55415-.166 1.0532-.40447 1.494-.71803.4282-.32278.8187-.72086 1.1713-1.1912.34005-.47956.68074-1.0323 1.0208-1.6594.12084-.22819.23523-.42557.3524-.63696l.0066.0017.0033-.005.0364-.06618h2.5296v.0017h1.125v-2.4072h-.33089v-.0017h-1.9787c.21975-.40713.43079-.78837.62041-1.1151.2141-.36889.45189-.6815.71637-.93972.26448-.25822.56659-.45561.90663-.59394.07046-.026754.15931-.041111.23824-.061214h.99266v-.00331h-.0017v-2.4105h-.97777v-.00165z" fill="#fff"/>
        </g>
      </svg>
    |},
  ),
  (
    "Onivim logo",
    {|
      <!-- Source: https://raw.githubusercontent.com/onivim/oni2/master/assets/images/logo.svg -->

      <svg id="Layer_1" data-name="Layer 1"
        xmlns="http://www.w3.org/2000/svg"
        xmlns:xlink="http://www.w3.org/1999/xlink" width="1023.62" height="990.06" viewBox="0 0 1023.62 990.06">
        <defs>
          <linearGradient id="linear-gradient" x1="495.41" y1="803.4" x2="495.41" y2="128.72" gradientUnits="userSpaceOnUse">
            <stop offset="0" stop-color="#e6537c"/>
            <stop offset="1" stop-color="#ffe8c7"/>
          </linearGradient>
        </defs>
        <title>logo</title>
        <g id="Icon">
          <g id="O_2" data-name="O&amp;2">
            <path d="M755.74,378.89a4.49,4.49,0,0,1-4.48-4.49V222.33a4.48,4.48,0,0,1,4.48-4.48H974.65V202.6a47.15,47.15,0,0,0-47.1-47.1H755.74a4.49,4.49,0,0,1-4.48-4.49V110.63a4.48,4.48,0,0,1,4.48-4.48H927.55A96.56,96.56,0,0,1,1024,202.6v60.11a4.48,4.48,0,0,1-4.49,4.48H800.61v62.35h174a49.17,49.17,0,0,1,49.1,44.42,4.48,4.48,0,0,1-4.46,4.93ZM990.44,512a496.53,496.53,0,0,0-9.51-96.51,7.37,7.37,0,0,0-7.23-5.93H951.11a7.37,7.37,0,0,0-7.22,8.87A460.16,460.16,0,0,1,953.6,512c0,252.65-205.54,458.19-458.19,458.19S37.22,764.68,37.22,512,242.76,53.84,495.41,53.84A459.15,459.15,0,0,1,709.74,107.1a7.37,7.37,0,0,0,10.82-6.51V75.91a7.37,7.37,0,0,0-4.06-6.58A488.6,488.6,0,0,0,495.41,17c-273,0-495,222.07-495,495s222.07,495,495,495S990.44,785,990.44,512Z" transform="translate(-0.38 -17)" style="fill:#3af5e9"/>
          </g>
          <path id="S" d="M499.5,845.25c-88.24,0-177.54-29.33-258.23-84.78-6-4.08-9.24-14.07-7.89-23.93s7-16.93,13.46-16.93H752.17c6.5,0,12.11,7.06,13.46,16.93s-2,19.85-7.89,23.93C677.05,815.92,587.75,845.25,499.5,845.25ZM194,647.67c-2.73,0-5.27-3-6.75-7.9-4.36-14.57-8.58-29.78-12.55-45.19a34.89,34.89,0,0,1,.12-17c1.44-5.22,4.06-8.44,6.89-16.1H817.83c2.83,7.66,5.45,10.88,6.89,16.1a34.77,34.77,0,0,1,.12,17c-4,15.41-8.19,30.62-12.55,45.19-1.47,4.93-4,7.9-6.75,7.9ZM162.11,497.22a7.39,7.39,0,0,1-7.35-7.85c11.66-178.76,161.29-318.79,340.65-318.79A341.32,341.32,0,0,1,718,253.14a7.36,7.36,0,0,1,2.56,5.59V374.41a35.22,35.22,0,0,0,35.18,35.18h59.88a7.35,7.35,0,0,1,7.06,5.27,340.84,340.84,0,0,1,13.37,74.5,7.37,7.37,0,0,1-7.35,7.86Z" transform="translate(-0.38 -17)" style="fill:url(#linear-gradient)"/>
        </g>
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
      flexGrow(1),
      position(`Relative),
    ];

    let buttons = [width(200), color(Colors.white), marginLeft(10)];

    let example = [width(350), height(350)];
  };

  let loadSVG = data => Ok(data);
  // switch (SVG.fromString(data)) {
  // | Some(svg) => Ok(svg)
  // | None => Error("Parse error: empty string")
  // | exception (Failure(msg)) => Error("Parse error: " ++ msg)
  // };

  let%component make = () => {
    let%hook (currentExample, setExample) = {
      let initialState =
        switch (examples) {
        | [(_, data), ..._] => loadSVG(data)
        | [] => Error("No examples!")
        };
      Hooks.state(initialState);
    };

    let buttons =
      List.map(
        ((text, value)) => RadioButtonsString.{text, value},
        examples,
      );
    let onChange = data => setExample(_ => loadSVG(data));

    // let example =
    //   switch (currentExample) {
    //   | Ok(svg) => <SVGString style=Styles.example contents=svg />
    //   | Error(message) => <Text text=message />
    //   };

    <View style=Styles.container>
      <Canvas
        style=Style.[
          position(`Absolute),
          top(0),
          left(0),
          bottom(0),
          right(0),
          backgroundColor(Revery.Colors.black),
        ]
        render={(canvasContext, _dimensions) => {
          switch (currentExample) {
          | Ok(svg) =>
            Skia.
              (
                {
                  let svgStr = {|
      <svg width="400" height="180">
        <rect x="50" y="20" width="150" height="150" style="fill:blue;stroke:pink;stroke-width:5;fill-opacity:0.1;stroke-opacity:0.9" />
      </svg>
    |};
                  let stream =
                    Stream.makeMemoryStreamWithData(
                      svgStr,
                      String.length(svgStr),
                    );

                  let svg = SVG.makeFromStream(stream);
                  SVG.setContainerSize(svg, 200., 200.);

                  Draw.CanvasContext.drawSVG(~svg, canvasContext);
                }
              )
              // let layer =
              //   Draw.CanvasContext.createLayer(
              //     ~forceCpu=true,
              //     ~width=1200l,
              //     ~height=1200l,
              //     canvasContext,
              //   );
              // prerr_endline(
              //   Printf.sprintf(
              //     "SVG: string container size: w=%f h=%f\n",
              //     SVG.getContainerWidth(svg),
              //     SVG.getContainerHeight(svg),
              //   ),
              // );
              // switch (layer) {
              // | None => ()
              // | Some(layer) =>
              //   prerr_endline("Trying to draw!");
              //   Draw.CanvasContext.drawSVG(~svg, layer);
              //   Draw.CanvasContext.flush(layer);
              //   let paint = Skia.Paint.make();
              //   Draw.CanvasContext.drawLayer(
              //     ~paint,
              //     ~layer,
              //     ~x=50.,
              //     ~y=50.,
              //     canvasContext,
              //   );
              // };
          | Error(_) => ()
          }
        }}
      />
    </View>;
  };
};

let render = () => <SVGExample />;
