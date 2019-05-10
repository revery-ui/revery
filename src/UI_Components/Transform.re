open Revery_UI;


module Rotate = {

let createElement = (~children, ~value, ()) => {
    let style = Style.[transform(Transform.Rotate(value))];

    <View style> ...children </View>;
				};
};

module TranslateY = {
let createElement = (~children, ~value, ()) => {
    let style = Style.[transform(Transform.TranslateY(value))];

    <View style> ...children </View>;
				};
};
