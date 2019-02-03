/*
  Like https://github.com/superbobry/ocaml-textwrap but with less options
  and with the `length` function passed as param (instead of `String.length`).
 */

let wordSeparationRe = Str.regexp("[\r\n\t ]+");
let whitespaceRe = Str.regexp("\t\n\011\012\r");
let sentenceEndRe = Str.regexp("[a-z][.!?][\"']?$");

/*
  Split the text to wrap into indivisible chunks. Currently
  the chunks are the same as words.
 */
let split = (_w, s) => {
  let res = Str.full_split(wordSeparationRe, s);
  List.map(
    fun
    | Str.Delim(_) => " "
    | Str.Text(t) => t,
    res,
  );
};

/*
  Handle a chunk of text (most likely a word, not whitespace) that
  is too long to fit in any line.
 */
let handleLongWord = (chunks, curLine) => {
  switch (chunks) {
  | [chunk, ...chunks] when curLine == [] => ([chunk, ...curLine], chunks)
  | chunks => (curLine, chunks)
  };
};

/*
  Wrap a sequence of text chunks and return a list of lines of
  length [maxWidth] or less.  Chunks correspond
  to words and the whitespace between them: each chunk is
  indivisible, but a line break can
  come between any two chunks.  Chunks should not have internal
  whitespace; ie. a chunk is either all whitespace or a "word".
  Whitespace chunks will be removed from the beginning and end of
  lines, but apart from that whitespace is preserved.
 */

let wrapChunks = (maxWidth, length) => {
  let rec current = (line, len, width) =>
    fun
    | [chunk, ...chunks] when length(chunk) + len <= width =>
      current([chunk, ...line], len + length(chunk), width, chunks)
    | chunks => (line, len, chunks);

  let rec inner = lines =>
    fun
    | [] => List.rev(lines)
    | chunks => {
        /* Fill current line with chunks to fit maximum width. */
        let (curLine, _curLen, chunks) = current([], 0, maxWidth, chunks);

        /* The current line is full, and the next chunk is too big to
           fit on *any* line (not just this one). */
        let (curLine, chunks) =
          if (chunks != [] && length(List.hd(chunks)) > maxWidth) {
            handleLongWord(chunks, curLine);
          } else {
            (curLine, chunks);
          };

        /* If the last chunk on this line is all whitespace -- drop it! */
        switch (curLine) {
        | [] => inner(lines, chunks)
        | curLine =>
          inner([String.concat("", List.rev(curLine)), ...lines], chunks)
        };
      };
  inner([]);
};

let wrap = (maxWidth, length, text) => {
  let chunks = split(maxWidth, text);
  let wrappedChunks = wrapChunks(maxWidth, length, chunks);
  String.concat("\n", wrappedChunks);
};