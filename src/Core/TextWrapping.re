type wrapType =
  | NoWrap
  | Wrap
  | WrapHyphenate;

let wrapText = (~text, ~measureWidth, ~maxWidth, ~mode) => {
  let module C: Wrap.TEXT_WRAP_CONFIG = {
    let width_of_char = measureWidth
    let debug = false;
  };
  let module W = Wrap.TextWrap(C);
  switch (mode) {
    | NoWrap =>
      [text]
    | Wrap =>
      W.wrap(text, ~max_width=maxWidth) |> Wrap.AppendList.to_list
    | WrapHyphenate =>
      W.wrap(text, ~max_width=maxWidth, ~hyphenate=true) |> Wrap.AppendList.to_list
  }
};
