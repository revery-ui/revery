open Revery_UI;

module Rotate = {
  let createElement = (~children, ~value, ()) => {
    let style = Style.[transform([TransformStyle.Rotate(value)])];

    <View style> ...children </View>;
  };
};
module RotateX = {
  let createElement = (~children, ~value, ()) => {
    let style = Style.[transform([TransformStyle.RotateX(value)])];

    <View style> ...children </View>;
  };
};
module RotateY = {
  let createElement = (~children, ~value, ()) => {
    let style = Style.[transform([TransformStyle.RotateY(value)])];

    <View style> ...children </View>;
  };
};

module RotateZ = {
  let createElement = (~children, ~value, ()) => {
    let style = Style.[transform([TransformStyle.RotateZ(value)])];

    <View style> ...children </View>;
  };
};

module TranslateX = {
  let createElement = (~children, ~value, ()) => {
    let style = Style.[transform([TransformStyle.TranslateX(value)])];

    <View style> ...children </View>;
  };
};

module TranslateY = {
  let createElement = (~children, ~value, ()) => {
    let style = Style.[transform([TransformStyle.TranslateY(value)])];

    <View style> ...children </View>;
  };
};
