type wrapType =
  | NoWrap
  | WhitespaceWrap;

let splitByWhitespace = text => {
  let (_a, _) =
    text
    |> Str.split(Str.regexp("[ \n\r\x0c\t]+"))
    |> List.fold_left(
        ((splitted, runningSumOffset), cur) => {
          /* TODO is it? */
          let whitespaceWidth = 1;
          let currentRunningSumOffset =
            runningSumOffset + String.length(cur) + whitespaceWidth;

          (
            [(cur, currentRunningSumOffset), ...splitted],
            currentRunningSumOffset,
          );
        },
        ([], 0),
      );
  _a;
};

let wrapText = (~logging=false, ~text, ~measureWidth, ~maxWidth, ~isWrappingPoint) => {
  let currWidth = ref(0);
  let currOffset = ref(0);
  let currLineOffset = ref(0);
  let currLineNumber = ref(0);
  let maxWidthLine = ref(0);
  let lines: ref(list(string)) = ref([]);
  
  String.iteri((i, ch) => {
    let a = isWrappingPoint(Char.escaped(ch));
    let isEnd = i == String.length(text) - 1;

    if (logging) {
      print_endline("TextNode:: iter:: char: " ++ Char.escaped(ch) ++ ", i: " ++ string_of_int(i));
      print_endline("TextNode:: iter:: currOffset: " ++ string_of_int(currOffset^) ++ ", currLineOffset: " ++ string_of_int(currLineOffset^));
    }

    if (a || isEnd) {
      let word = String.sub(text, currOffset^, i - currOffset^ + 1);
      let dWidth = measureWidth(word);
      
      if (logging)
          print_endline("TextNode:: iter:: word: " ++ word ++ ", width: " ++ string_of_int(dWidth));

      if ((currWidth^ + dWidth) >= maxWidth || isEnd) {
        let line = if (isEnd)
          String.sub(text, currLineOffset^, String.length(text) - currLineOffset^)
        else
          String.sub(text, currLineOffset^, currOffset^ - currLineOffset^ - 1)

        if (logging)
          print_endline("TextNode:: line: " ++ line);

        lines := [line, ...lines^];
        currLineNumber := currLineNumber^ + 1;
        currLineOffset := currLineOffset^ + currOffset^ - currLineOffset^ - 1;
        currWidth := 0;

        if (maxWidthLine^ < currWidth^) {
          maxWidthLine:= currWidth^;
          ();
        } else {
          ();
        }
      } else {
        currOffset := currOffset^ + String.length(word);
        currWidth := currWidth^ + dWidth;
        ();
      }
    } else {
      ();  
    }
  }, text);

  (lines^, maxWidthLine^);
}