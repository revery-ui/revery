include Timber;

let enablePrinting = () => {
  // Additional utilities for logging in Revery
  let buffer = Buffer.create(0);
  let formatter = Format.make_formatter(Buffer.add_substring(buffer), () => {
  // We use `Console.log` instead of `print_endline` / default formatter to work around:
  // https://github.com/ocaml/ocaml/issues/9252
    Console.log(Buffer.contents(buffer));
    Buffer.clear(buffer);
  });

  Logs.format_reporter(~app=formatter, ~dst=formatter, ())
  |> Logs.set_reporter;

  Timber.App.enablePrinting();
};

let enableDebugLogging = Timber.App.enableDebugLogging;

