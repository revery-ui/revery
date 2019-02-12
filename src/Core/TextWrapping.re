type wrapType =
  | NoWrap
  | WhitespaceWrap
  | UserDefined((string, string => int, int) => (list(string), int));

type folder = {
  lines: list(string),
  currMaxWidth: int,
  beginIndex: int,
  endIndex: int,
};

let wrapText = (~logging=false, ~text, ~measureWidth, ~maxWidth, ~wrapHere) => {
  let textLength = String.length(text);

  let isEnd = i => textLength == i + 1;

  let subAndMeasure = (beginIndex, endIndex) => {
    let substr = String.sub(text, beginIndex, endIndex - beginIndex + 1);
    let substrWidth = measureWidth(substr);
    (substr, substrWidth);
  };

  let foldFunc = (acc, (index, char)) =>
    if (isEnd(index)) {
      print_endline(
        "is end:: beginIndex: "
        ++ string_of_int(acc.beginIndex)
        ++ " endIndex: "
        ++ string_of_int(acc.endIndex),
      );
      let currEndIndex = index;
      let (_substr, width) = subAndMeasure(acc.beginIndex, currEndIndex);

      if (width >= maxWidth) {
        print_endline("is end, width needs wrapping: ");
        let (line, lineWidth) = subAndMeasure(acc.beginIndex, acc.endIndex);
        let (lastLine, lastLineWidth) =
          subAndMeasure(acc.endIndex + 2, index);

        let currMaxWidth =
          max(lastLineWidth, max(acc.currMaxWidth, lineWidth));

        {...acc, lines: [lastLine, line, ...acc.lines], currMaxWidth};
      } else {
        let (line, lineWidth) = subAndMeasure(acc.beginIndex, index);

        print_endline("is end, width is ok:: line: " ++ line);
        {
          ...acc,
          lines: [line, ...acc.lines],
          currMaxWidth: max(acc.currMaxWidth, lineWidth),
        };
      };
    } else if (!wrapHere(Char.escaped(char))) {
      acc;
    } else {
      let currEndIndex = index - 1;
      let (_substr, width) = subAndMeasure(acc.beginIndex, currEndIndex);

      if (width >= maxWidth) {
        let beginningOfCurrentWordIndex = acc.endIndex + 2;
        let (lineWithoutCurrentWord, lineWithoutCurrentWordWidth) =
          subAndMeasure(acc.beginIndex, acc.endIndex);

        print_endline(
          "wrapping point, width needs wrapping:: lineWithoutCurrentWord: '"
          ++ lineWithoutCurrentWord
          ++ "' beginIndex: "
          ++ string_of_int(acc.beginIndex)
          ++ " endIndex: "
          ++ string_of_int(acc.endIndex)
          ++ " newBeginIndex: "
          ++ string_of_int(beginningOfCurrentWordIndex)
          ++ " char: "
          ++ Char.escaped(char)
          ++ " index: "
          ++ string_of_int(index),
        );
        {
          lines: [lineWithoutCurrentWord, ...acc.lines],
          currMaxWidth: max(acc.currMaxWidth, lineWithoutCurrentWordWidth),
          beginIndex: beginningOfCurrentWordIndex,
          endIndex: currEndIndex,
        };
      } else {
        print_endline(
          "wrapping point, width is ok:: currEndIndex: "
          ++ string_of_int(currEndIndex)
          ++ " beginIndex: "
          ++ string_of_int(acc.beginIndex)
          ++ " endIndex: "
          ++ string_of_int(acc.endIndex)
          ++ " char: "
          ++ Char.escaped(char)
          ++ " index: "
          ++ string_of_int(index),
        );
        {...acc, endIndex: currEndIndex};
      };
    };

  logging |> ignore;

  let folda =
    String.to_seqi(text)
    |> Seq.fold_left(
         foldFunc,
         {lines: [], currMaxWidth: 0, beginIndex: 0, endIndex: 0},
       );

  (List.rev(folda.lines), folda.currMaxWidth);
};
