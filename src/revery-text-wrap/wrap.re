open Tokenize;
module Tokenize = Tokenize;

let rec list_of_queue = q =>
  if (Queue.is_empty(q)) {
    [];
  } else {
    /* We need to split into two lines because the RHS of ::
       gets evaluated first, leading to recursion before the
       item is removed from the queue. */
    let first = Queue.take(q);
    [first, ...list_of_queue(q)];
  };

[@inline always]
let last_uchar_of_string = str =>
  switch (str) {
  | "" => Uchar.of_int(0)
  | _ =>
    let offset = Zed_utf8.prev(str, String.length(str));
    let offset = offset == String.length(str) ? 0 : offset;
    Zed_utf8.extract(str, offset);
  };

let wrap_queue =
    (
      ~max_width,
      ~width_of_token,
      ~hyphenate=false,
      ~ignore_preceding_whitespace=true,
      ~debug=false,
      text,
    ) => {
  /* Create a buffer for the outputted lines */
  let output_lines = Queue.create();
  /* Split the input text into lines and for each line: */
  split_on_newline(text)
  |> Queue.iter(line => {
       /* Create a buffer for the wrapped portion of the line */
       let buffer = Buffer.create(String.length(line));
       /* Store the width of this portion */
       let width = ref(0.0);
       /* Tokenize the line by whitespace and for each token: */
       let x = split_tokens(line);
       x
       |> Queue.iter(token => {
            /* Calculate the width of the token */
            let token_width = width_of_token(token);

            if (debug) {
              Printf.printf(
                "Token: %s, Width: %f, Token_width: %f, Max_width: %f\n",
                token,
                width^,
                token_width,
                max_width,
              );
            };

            /* If the buffer is already too long, push the buffer and reset it */
            if (width^ >= max_width) {
              if (debug) {
                print_endline("Clear");
              };
              Queue.add(Buffer.contents(buffer), output_lines);
              Buffer.reset(buffer);
              width := 0.0;
            };

            /* Check to see if the line starts w/ whitespace and if we should ignore it */
            if (ignore_preceding_whitespace
                && is_whitespace(token)
                && width^ == 0.0) {
              if (debug) {
                print_endline("Decision: ignore");
              };
              ();
              /* If it's gonna be too long with the whitespace, don't add it & just prepare to wrap,
                 so that we don't get the whitespace at the start of the next line. */
            } else if (ignore_preceding_whitespace
                       && is_whitespace(token)
                       && width^
                       +. token_width > max_width) {
              if (debug) {
                print_endline("Decision: append-whitespace");
              };
              width := width^ +. token_width;
              /* If we can add the token without exceeding the limit, add it to the current line */
            } else if (width^ +. token_width <= max_width) {
              if (debug) {
                print_endline("Decision: append (" ++ __LOC__ ++ ")");
              };
              width := width^ +. token_width;
              Buffer.add_string(buffer, token);
              /* If it would exceed the limit and the user wants hyphenation: */
            } else if (hyphenate && width^ +. token_width > max_width) {
              if (debug) {
                print_endline("Decision: hyphenate");
              };

              let rec loop = (~str, ~offset, ~iteration as i) =>
                if (offset != String.length(str)) {
                  let (uchar, nextOffset) =
                    Zed_utf8.extract_next(str, offset);

                  let char_width = width_of_token(Zed_utf8.singleton(uchar));

                  /* If it won't overflow this line OR if there's no way to fit it into a line without overflowing */
                  if (width^
                      +. char_width <= max_width
                      || width^ == 0.0
                      && char_width >= max_width) {
                    if (debug) {
                      print_endline("--Decision: append (" ++ __LOC__ ++ ")");
                    };
                    /* Append it to the buffer */
                    Buffer.add_utf_8_uchar(buffer, uchar);
                    width := width^ +. char_width;
                    /* If it will overflow... */
                  } else {
                    if (debug) {
                      print_endline("--Decision: break");
                    };
                    /* Finalize the current line and reset the buffer (if we need to) */
                    if (width^ > 0.0) {
                      /* If we're part-way through the string already */
                      if (i > 0) {
                        if (debug) {
                          print_endline("--Clear+hyphenate");
                        };
                        /* We need to swap the last char of the buffer with a -, because the
                           hyphen will be the last character that fits into this width. */
                        let buffer_str = Buffer.contents(buffer);
                        let last_uchar = last_uchar_of_string(buffer_str);
                        /* If we've only put one character of the string before hyphenating, we
                           should just swap with a space, so that we don't have a lonely hyphen
                           on the previous line */
                        let hyphen =
                          if (i == 1) {
                            " ";
                          } else {
                            "-";
                          };
                        /* Flush the buffer with the hyphen and reset the buffer to just the last
                           character that was in the buffer (where the hyphen now is) */
                        Queue.add(
                          Zed_utf8.rchop(buffer_str) ++ hyphen,
                          output_lines,
                        );
                        Buffer.reset(buffer);
                        Buffer.add_utf_8_uchar(buffer, last_uchar);
                        width := width_of_token(Zed_utf8.singleton(uchar));
                        /* Otherwise, this is the start of the token */
                      } else {
                        if (debug) {
                          print_endline("--Clear");
                        };
                        /* So just flush & reset the buffer */
                        Queue.add(Buffer.contents(buffer), output_lines);
                        Buffer.reset(buffer);
                        width := 0.0;
                      };
                    };
                    /* Then push the next character from this token onto the buffer */
                    width := width^ +. char_width;
                    Buffer.add_utf_8_uchar(buffer, uchar);
                  };
                  loop(~str, ~offset=nextOffset, ~iteration=i + 1);
                };

              loop(~str=token, ~offset=0, ~iteration=0);
              /* If it would exceed the limit and the user doesn't want hyphenation: */
            } else {
              if (debug) {
                print_endline("Decision: wrap");
              };
              /* Finalize the current line and reset the buffer (if we need to) */
              if (width^ > 0.0) {
                Queue.add(Buffer.contents(buffer), output_lines);
                Buffer.reset(buffer);
              };
              /* Then push the new token onto the buffer */
              width := token_width;
              Buffer.add_string(buffer, token);
            };
          });
       /* Finalize any remaining text in the buffer */
       if (width^ > 0.0) {
         Queue.add(Buffer.contents(buffer), output_lines);
       };
     });
  output_lines;
};

let wrap =
    (
      ~max_width,
      ~width_of_token,
      ~hyphenate=false,
      ~ignore_preceding_whitespace=true,
      ~debug=false,
      text,
    ) =>
  list_of_queue(
    wrap_queue(
      ~max_width,
      ~width_of_token,
      ~hyphenate,
      ~ignore_preceding_whitespace,
      ~debug,
      text,
    ),
  );
