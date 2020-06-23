module FontDescriptor = FontDescriptor;
module FontWeight = FontWeight;
module FontWidth = FontWidth;

exception Font_not_found;
let findFont:
  (
    ~weight: FontWeight.t=?,
    ~width: FontWidth.t=?,
    ~family: string,
    ~italic: bool,
    ~mono: bool,
    unit
  ) =>
  FontDescriptor.t;
let findFontOpt:
  (
    ~weight: FontWeight.t=?,
    ~width: FontWidth.t=?,
    ~family: string,
    ~italic: bool,
    ~mono: bool,
    unit
  ) =>
  option(FontDescriptor.t);
