open Revery_TextWrap;

/* Get the width of a character */
let width_of_token = str => {
  String.length(str) |> float;
};

Timber.App.enable();
Timber.App.setLevel(Timber.Level.trace);

Printexc.record_backtrace(true);

let () = {
  wrap(~width_of_token, ~max_width=100.0, "0") |> List.iter(print_endline);
  let wrapInput =
    "Here's another example of text where wrapping might be more difficult. "
    ++ "This string is very, very long and consists of words of varying lengths. "
    ++ "By utilizing some extremely long words, we can hopefully trigger some of "
    ++ "the more obscure edge-cases that word-wrapping can result in, such as "
    ++ "placing a hyphen in the middle of a word on top of another hyphen."
    ++ "Let's also throw some UTF8 in here, who doesn't like a good emoji or 10?"
    ++ "😀 🤔 🤭 🤫 🤥 😶 😐 😑 😬"
    ++ "Also let's try some CJK!"
    ++ "日本語の場合はランダムに生成された文章以外に、 著作権が切れた小説などが利用されることもある。";
  print_endline("==================");
  wrap(~width_of_token, ~max_width=5.0, ~hyphenate=true, wrapInput)
  |> List.iter(print_endline);
  print_endline("==================");
  wrap(~width_of_token, ~max_width=10.0, ~hyphenate=true, wrapInput)
  |> List.iter(print_endline);
  print_endline("==================");
  wrap(~width_of_token, ~max_width=15.0, wrapInput)
  |> List.iter(print_endline);
  print_endline("==================");
  wrap(~width_of_token, ~max_width=40.0, wrapInput)
  |> List.iter(print_endline);
};
