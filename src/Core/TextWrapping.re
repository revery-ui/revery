open Revery_TextWrap;

type wrapType =
  | NoWrap
  | Wrap
  | WrapIgnoreWhitespace
  | WrapHyphenate;

let wrapText = (~text, ~measureWidth as width_of_token, ~maxWidth, ~mode) => {
  switch (mode) {
  | NoWrap => [text]
  | Wrap => Wrap.wrap(text, ~max_width=maxWidth, ~width_of_token)
  | WrapIgnoreWhitespace =>
    Wrap.wrap(
      text,
      ~max_width=maxWidth,
      ~width_of_token,
      ~ignore_preceding_whitespace=false,
    )
  | WrapHyphenate =>
    Wrap.wrap(text, ~max_width=maxWidth, ~width_of_token, ~hyphenate=true)
  };
};
