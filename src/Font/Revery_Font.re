/**
    Revery_Font.re

    Module exposing font-related functionality, like:
    - Discovering fonts
    - Loading fonts
*/

module FontWeight = FontManager.FontWeight;
module FontWidth = FontManager.FontWidth;

type t = FontManager.FontDescriptor.t;

let find = (
    ~weight=FontWeight.Bold, 
    ~width=FontWidth.Undefined,
    ~mono=false,
    ~italic=false,
    family) => 
        FontManager.findFont(~weight, ~width, ~mono, ~italic, ~family, ());
        
let show = FontManager.FontDescriptor.show;
