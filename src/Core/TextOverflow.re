type t =
  | Overflow
  | Clip
  | Ellipsis
  | UserDefined(string);

let removeLineBreaks = text => {
  let re = Str.regexp("\n");
  Str.global_replace(re, "", text);
};

let rec handleOverflow = (~maxWidth, ~text, ~measure, ~character="…", ()) => {
  let clippedText = String.length(text) - 1 |> Str.string_before(text);

  let width = measure(clippedText ++ character);
  width >= maxWidth && String.length(clippedText) > 1
    ? handleOverflow(~maxWidth, ~text=clippedText, ~measure, ~character, ())
    : clippedText ++ character;
};
