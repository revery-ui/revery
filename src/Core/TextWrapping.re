type wrapType =
  | NoWrap
  | Wrap
  | WrapHyphenate;

let wrapText = (~text, ~measureWidth, ~maxWidth, ~mode) => {
  let rec list_of_queue = q =>
    if (Queue.is_empty(q)) {
      []
    } else {
      /* We need to split into two lines because the RHS of ::
         gets evaluated first, leading to recursion before the
         item is removed from the queue. */
      let first = Queue.take(q);
      [first, ...list_of_queue(q)]
    };
  switch (mode) {
    | NoWrap =>
      [text]
    | Wrap =>
      Wrap.wrap(text, ~max_width=maxWidth, ~width_of_char=measureWidth) |> list_of_queue
    | WrapHyphenate =>
      Wrap.wrap(text, ~max_width=maxWidth, ~width_of_char=measureWidth, ~hyphenate=true) |> list_of_queue
  }
};
