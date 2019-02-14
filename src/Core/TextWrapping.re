type wrapType =
  | NoWrap
  | WhitespaceWrap
  | UserDefined((string, string => int, int) => (list(string), int));

type linesFolderAccumulator = {
  lines: list(string),
  currMaxWidth: int,
  beginIndex: int,
  endIndex: int,
};

/* See https://www.fileformat.info/info/unicode/category/Zs/list.htm */
let space = " ";
let nbsp = "\xa0";

/* TODO Because we're not using regex for space matching we should take all kinds of spaces into conderation
   However, I can't make unicode escape work, it's giving me 'Warning 14: illegal backslash escape in string.' error
   */

/*
 let oghamSpace = "\u{1680}";
 let enQuad = "\u{2000}";
 let emQuad = "\u{2001}";
 let enSpace = "\u{2002}";
 let emSpace = "\u{2003}";
 let threePerEmSpace = "\u{2004}";
 let fourPerEmSpace = "\u{2005}";
 let sixPerEmSpace = "\u{2006}";
 let figureSpace = "\u{2007}";
 let punctuationSpace = "\u{2008}";
 let thinSpace = "\u{2009}";
 let hairSpace = "\u{200A}";
 let narrowNbsp = "\u{202f}";
 let mediumMathSpace = "\u{205f}";
 let ideographicSpace = "\u{3000}";
 */
let lineBreak = "\n";
let tab = "\t";
let lineFeed = "\x0c";

let isWhitespaceWrapPoint = str =>
  str != nbsp
  && (str == space || str == lineBreak || str == lineFeed || str == tab);

let wrapText = (~text, ~measureWidth, ~maxWidth, ~wrapHere) => {
  let textLength = String.length(text);

  let isEnd = i => textLength == i + 1;

  let subAndMeasure = (beginIndex, endIndex) => {
    let substr = String.sub(text, beginIndex, endIndex - beginIndex + 1);
    let substrWidth = measureWidth(substr);
    (substr, substrWidth);
  };

  let foldIntoLines = (acc, (index, char)) =>
    if (isEnd(index)) {
      let currEndIndex = index;
      let (_substr, width) = subAndMeasure(acc.beginIndex, currEndIndex);

      if (width >= maxWidth) {
        let (line, lineWidth) = subAndMeasure(acc.beginIndex, acc.endIndex);
        let (lastLine, lastLineWidth) =
          subAndMeasure(acc.endIndex + 2, index);

        let currMaxWidth =
          max(lastLineWidth, max(acc.currMaxWidth, lineWidth));

        {...acc, lines: [lastLine, line, ...acc.lines], currMaxWidth};
      } else {
        let (line, lineWidth) = subAndMeasure(acc.beginIndex, index);

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

        {
          lines: [lineWithoutCurrentWord, ...acc.lines],
          currMaxWidth: max(acc.currMaxWidth, lineWithoutCurrentWordWidth),
          beginIndex: beginningOfCurrentWordIndex,
          endIndex: currEndIndex,
        };
      } else {
        {...acc, endIndex: currEndIndex};
      };
    };

  let linesAndMaxWidth =
    String.to_seqi(text)
    |> Seq.fold_left(
         foldIntoLines,
         {lines: [], currMaxWidth: 0, beginIndex: 0, endIndex: 0},
       );

  (List.rev(linesAndMaxWidth.lines), linesAndMaxWidth.currMaxWidth);
};
