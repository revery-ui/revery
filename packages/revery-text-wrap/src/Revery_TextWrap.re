open Tokenize;

module Log = (val Timber.Log.withNamespace("Revery.TextWrap"));

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

let log10_of_2 = log10(2.);

[@inline always]
let uchar_num_bytes = uchar => {
  let code = Uchar.to_int(uchar);
  let num_bits = log10(float(code)) /. log10_of_2;
  num_bits /. 8. |> int_of_float;
};

let wrap_queue =
    (
      ~max_width,
      ~width_of_token,
      ~hyphenate=false,
      ~ignore_preceding_whitespace=true,
      text,
    ) => {
  /* Create a buffer for the outputted lines */
  let output_lines = Queue.create();
  /* Split the input text into lines and for each line: */
  split_on_newline(text)
  |> Queue.iter(line => {
       /* Create a buffer for the wrapped portion of the line */
       let buffer = Buffer.create(String.length(line));
       let current_offset = ref(0);
       let last_added = ref(None);

       let buffer_add_uchar = uchar => {
         let num_bytes = uchar_num_bytes(uchar);
         Buffer.add_utf_8_uchar(buffer, uchar);
         current_offset := current_offset^ + num_bytes;
         last_added := Some(`Uchar(uchar));
       };

       let buffer_add_string = str => {
         let num_bytes = String.length(str);
         Buffer.add_string(buffer, str);
         current_offset := current_offset^ + num_bytes;
         last_added := Some(`String(str));
       };

       let buffer_reset = () => {
         Buffer.reset(buffer);
         current_offset := 0;
         last_added := None;
       };

       let buffer_last_uchar = () =>
         switch (last_added^) {
         | Some(`Uchar(uchar)) => uchar
         | Some(`String(str)) => last_uchar_of_string(str)
         | None => Uchar.of_int(0)
         };

       /* Store the width of this portion */
       let width = ref(0.0);
       /* Tokenize the line by whitespace and for each token: */
       split_tokens(line)
       |> Queue.iter(token => {
            /* Calculate the width of the token */
            let token_width = width_of_token(token);

            Log.tracef(m =>
              m(
                "Token: %s, Width: %f, Token_width: %f, Max_width: %f\n",
                token,
                width^,
                token_width,
                max_width,
              )
            );

            /* If the buffer is already too long, push the buffer and reset it */
            if (width^ >= max_width) {
              Log.trace("Clear");
              Queue.add(Buffer.contents(buffer), output_lines);
              buffer_reset();
              width := 0.0;
            };

            /* Check to see if the line starts w/ whitespace and if we should ignore it */
            if (ignore_preceding_whitespace
                && is_whitespace(token)
                && width^ == 0.0) {
              Log.trace(
                "Decision: ignore",
                /* If it's gonna be too long with the whitespace, don't add it & just prepare to wrap,
                   so that we don't get the whitespace at the start of the next line. */
              );
            } else if (ignore_preceding_whitespace
                       && is_whitespace(token)
                       && width^
                       +. token_width > max_width) {
              Log.trace("Decision: append-whitespace");
              width := width^ +. token_width;
              /* If we can add the token without exceeding the limit, add it to the current line */
            } else if (width^ +. token_width <= max_width) {
              Log.tracef(m => m("Decision: append (%s)", __LOC__));
              width := width^ +. token_width;
              buffer_add_string(token);
              /* If it would exceed the limit and the user wants hyphenation: */
            } else if (hyphenate && width^ +. token_width > max_width) {
              Log.trace("Decision: hyphenate");

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
                    Log.tracef(m => m("--Decision: append (%s)", __LOC__));
                    /* Append it to the buffer */
                    buffer_add_uchar(uchar);
                    width := width^ +. char_width;
                    /* If it will overflow... */
                  } else {
                    Log.trace("--Decision: break");
                    /* Finalize the current line and reset the buffer (if we need to) */
                    if (width^ > 0.0) {
                      /* If we're part-way through the string already */
                      if (i > 0) {
                        Log.trace("--Clear+hyphenate");
                        /* We need to swap the last char of the buffer with a -, because the
                           hyphen will be the last character that fits into this width. */
                        let last_uchar = buffer_last_uchar();
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
                          Zed_utf8.rchop(Buffer.contents(buffer)) ++ hyphen,
                          output_lines,
                        );
                        buffer_reset();
                        buffer_add_uchar(last_uchar);
                        width := width_of_token(Zed_utf8.singleton(uchar));
                        /* Otherwise, this is the start of the token */
                      } else {
                        Log.trace("--Clear");
                        /* So just flush & reset the buffer */
                        Queue.add(Buffer.contents(buffer), output_lines);
                        buffer_reset();
                        width := 0.0;
                      };
                    };
                    /* Then push the next character from this token onto the buffer */
                    width := width^ +. char_width;
                    buffer_add_uchar(uchar);
                  };
                  loop(~str, ~offset=nextOffset, ~iteration=i + 1);
                };

              loop(~str=token, ~offset=0, ~iteration=0);
              /* If it would exceed the limit and the user doesn't want hyphenation: */
            } else {
              Log.trace("Decision: wrap");
              /* Finalize the current line and reset the buffer (if we need to) */
              if (width^ > 0.0) {
                Queue.add(Buffer.contents(buffer), output_lines);
                buffer_reset();
              };
              /* Then push the new token onto the buffer */
              width := token_width;
              buffer_add_string(token);
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
      text,
    ) =>
  list_of_queue(
    wrap_queue(
      ~max_width,
      ~width_of_token,
      ~hyphenate,
      ~ignore_preceding_whitespace,
      text,
    ),
  );
