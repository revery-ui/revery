open Tokenize
module Tokenize = Tokenize

(* Get the width of a token (i.e. the sum of each character's width) *)
let width_of_token ~width_of_char str =
  let sum = ref 0.0 in
  str |> String.iter begin fun char ->
    sum := !sum +. width_of_char char
  end;
  !sum

let rec list_of_queue q =
  if Queue.is_empty q then []
  else begin
    (* We need to split into two lines because the RHS of ::
        gets evaluated first, leading to recursion before the
        item is removed from the queue. *)
    let first = Queue.take q in
    first::(list_of_queue q)
  end

let wrap_queue ~max_width ~width_of_char ?(hyphenate=false) ?(ignore_preceding_whitespace=true) ?(debug=false) text =
  (* Create a buffer for the outputted lines *)
  let output_lines = Queue.create () in
  (* Split the input text into lines and for each line: *)
  split_on_newline text |> Queue.iter begin fun line ->
    (* Create a buffer for the wrapped portion of the line *)
    let buffer = Buffer.create (String.length line) in
    (* Store the width of this portion *)
    let width = ref 0.0 in
    (* Tokenize the line by whitespace and for each token: *)
    split_tokens line |> Queue.iter begin fun token ->
      (* Calculate the width of the token *)
      let token_width = width_of_token ~width_of_char token in

      if debug then begin
        Printf.printf "Token: %s, Width: %f, Token_width: %f, Max_width: %f\n"
          token !width token_width max_width
      end;

      (* If the buffer is already too long, push the buffer and reset it *)
      if !width >= max_width then begin
        if debug then begin
          print_endline "Clear"
        end;
        Queue.add (Buffer.contents buffer) output_lines;
        Buffer.reset buffer;
        width := 0.0
      end;

      (* Check to see if the line starts w/ whitespace and if we should ignore it *)
      if ignore_preceding_whitespace && is_whitespace token && !width = 0.0 then begin
        if debug then begin
          print_endline "Decision: ignore"
        end;
        ()
      (* If it's gonna be too long with the whitespace, don't add it & just prepare to wrap,
        so that we don't get the whitespace at the start of the next line. *)
      end else if ignore_preceding_whitespace && is_whitespace token
            && !width +. token_width > max_width then begin
        if debug then begin
          print_endline "Decision: append-whitespace"
        end;
        width := !width +. token_width
      (* If we can add the token without exceeding the limit, add it to the current line *)
      end else if !width +. token_width <= max_width then begin
        if debug then begin
          print_endline "Decision: append"
        end;
        width := !width +. token_width;
        Buffer.add_string buffer token
      (* If it would exceed the limit and the user wants hyphenation: *)
      end else if hyphenate && !width +. token_width > max_width then begin
        if debug then begin
          print_endline "Decision: hyphenate"
        end;

        (* For each character in the token, do processing to determine where to break *)
        for i = 0 to String.length token - 1 do
          let char_width = width_of_char token.[i] in
          if debug then begin
            Printf.printf "--Index: %d, Char: %c, Char_width: %f\n" i token.[i] char_width
          end;

          (* If it won't overflow this line OR if there's no way to fit it into a line without overflowing *)
          if (!width +. char_width <= max_width) || (!width = 0.0 && char_width >= max_width) then begin
            if debug then begin
              print_endline "--Decision: append"
            end;
            (* Append it to the buffer *)
            Buffer.add_char buffer token.[i];
            width := !width +. char_width
          (* If it will overflow... *)
          end else begin
            if debug then begin
              print_endline "--Decision: break"
            end;
            (* Finalize the current line and reset the buffer (if we need to) *)
            if !width > 0.0 then begin
              (* If we're part-way through the string already *)
              if i > 0 then begin
                if debug then begin
                  print_endline "--Clear+hyphenate"
                end;
                (* We need to swap the last char of the buffer with a -, because the
                    hyphen will be the last character that fits into this width. *)
                let buffer_length = Buffer.length buffer in
                let last_char = Buffer.nth buffer (buffer_length - 1) in
                (* If we've only put one character of the string before hyphenating, we
                    should just swap with a space, so that we don't have a lonely hyphen
                    on the previous line *)
                let hyphen = if i = 1 then " " else "-" in
                (* Flush the buffer with the hyphen and reset the buffer to just the last
                    character that was in the buffer (where the hyphen now is) *)
                Queue.add (Buffer.sub buffer 0 (buffer_length - 1) ^ hyphen) output_lines;
                Buffer.reset buffer;
                Buffer.add_char buffer last_char;
                width := width_of_char last_char
              (* Otherwise, this is the start of the token *)
              end else begin
                if debug then begin
                  print_endline "--Clear"
                end;
                (* So just flush & reset the buffer *)
                Queue.add (Buffer.contents buffer) output_lines;
                Buffer.reset buffer;
                width := 0.0
              end
            end;
            (* Then push the next character from this token onto the buffer *)
            width := !width +. char_width;
            Buffer.add_char buffer token.[i]
          end
        done
      (* If it would exceed the limit and the user doesn't want hyphenation: *)
      end else begin
        if debug then begin
          print_endline "Decision: wrap"
        end;
        (* Finalize the current line and reset the buffer (if we need to) *)
        if !width > 0.0 then begin
          Queue.add (Buffer.contents buffer) output_lines;
          Buffer.reset buffer
        end;
        (* Then push the new token onto the buffer *)
        width := token_width;
        Buffer.add_string buffer token
      end
    end;
    (* Finalize any remaining text in the buffer *)
    if !width > 0.0 then begin
      Queue.add (Buffer.contents buffer) output_lines
    end
  end;
  output_lines

let wrap ~max_width ~width_of_char ?(hyphenate=false) ?(ignore_preceding_whitespace=true) ?(debug=false) text =
  list_of_queue (wrap_queue ~max_width ~width_of_char ~hyphenate ~ignore_preceding_whitespace ~debug text)
