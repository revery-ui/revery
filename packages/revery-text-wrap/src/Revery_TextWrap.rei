let wrap:
  (
    ~max_width: float,
    ~width_of_token: string => float,
    ~hyphenate: bool=?,
    ~ignore_preceding_whitespace: bool=?,
    ~debug: bool=?,
    string
  ) =>
  list(string);
