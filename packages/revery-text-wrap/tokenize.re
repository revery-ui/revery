let slice = (~first, ~last, string) => {
  let length = last - first;
  String.sub(string, first, length);
};

/* Split a string into an AppendList of strings, chopping at every newline and
   removing the newline characters. */
let split_on_newline = string => {
  let lines = Queue.create();
  let first = ref(0);
  for (i in 0 to String.length(string) - 1) {
    if (string.[i] == '\n') {
      Queue.add(slice(~first=first^, ~last=i, string), lines);
      first := i + 1;
    };
  };
  /* Append the remaining characters after the last split */
  if (first^ <= String.length(string) - 1) {
    Queue.add(
      slice(~first=first^, ~last=String.length(string), string),
      lines,
    );
  };
  lines;
};

/* Tokenize a string into an AppendList of strings, chopping on every instance of whitespace
   and every hyphen. Isolate whitespace into its own token and preserve it in the final list. */
let split_tokens = string => {
  let tokens = Queue.create();
  let first = ref(0);
  for (i in 0 to String.length(string) - 1) {
    switch (string.[i]) {
    | '\n'
    | ' '
    | '\t' =>
      /* Push everything before this character (if there is anything) */
      if (first^ != i) {
        Queue.add(slice(~first=first^, ~last=i, string), tokens);
      };
      /* Then push this character */
      Queue.add(slice(~first=i, ~last=i + 1, string), tokens);
      /* And skip it */
      first := i + 1;
    | '-' =>
      /* When a word is already hyphenated, split it into tokens so we don't needlessly
         rehyphenate it and end up with a double hyphen */
      Queue.add(slice(~first=first^, ~last=i + 1, string), tokens);
      first := i + 1;
    | _ => ()
    };
  };
  /* Append the remaining characters after the last split */
  if (first^ <= String.length(string) - 1) {
    Queue.add(
      slice(~first=first^, ~last=String.length(string), string),
      tokens,
    );
  };
  tokens;
};

/* Check if a token is blank/whitespace */
let is_whitespace = str =>
  /* We only have to check the start b/c the tokenizer won't return anything after the whitespace */
  String.length(str) == 0 || str.[0] === ' ' || str.[0] === '\t';
