type wrapType =
  | NoWrap
  | Wrap
  | WrapHyphenate;

let wrapText = (~text, ~measureWidth, ~maxWidth, ~mode) => {
  switch (mode) {
  | NoWrap => [text]
  | Wrap => Wrap.wrap(text, ~max_width=maxWidth, ~width_of_char=measureWidth)
  | WrapHyphenate =>
    Wrap.wrap(
      text,
      ~max_width=maxWidth,
      ~width_of_char=measureWidth,
      ~hyphenate=true,
    )
  };
};
