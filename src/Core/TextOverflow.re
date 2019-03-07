type t =
  | Overflow
  | Ellipsis
  | UserDefined(string);

let unwrapText = text => {
  let re = Str.regexp("\n");
  Str.global_replace(re, "", text);
};

let rec handleOverflow = (~parentWidth, ~text, ~measure, ~character="…", ()) => {
  let clippedText =
    String.length(text) - 1 |> Str.string_before(text) |> (++)(character);
  let width = measure(clippedText);

  width >= parentWidth ?
    handleOverflow(
      ~parentWidth,
      ~text=clippedText,
      ~measure,
      ~character,
      (),
    ) :
    clippedText;
};
