type os =
  | Windows
  | Mac
  | Linux
  | Unknown;

let uname = () => {
  let ic = Unix.open_process_in("uname");
  let uname = input_line(ic);
  let () = close_in(ic);
  uname;
};

let get_os =
  switch (Sys.os_type) {
  | "Win32" => Windows
  | _ =>
    switch (uname()) {
    | "Darwin" => Mac
    | "Linux" => Linux
    | _ => Unknown
    }
  };

let root = Sys.getenv("cur__root");
let c_flags = [];

let ccopt = s => ["-ccopt", s];
let cclib = s => ["-cclib", s];

let flags =
  switch (get_os) {
  | Windows => [] @ cclib("-ldwrite") @ cclib("-lstdc++")
  | Linux =>
    [] @ cclib("-lfontconfig") @ cclib("-lstdc++") @ ccopt("-I/usr/include")
  | Mac =>
    []
    @ ccopt("-framework CoreText")
    @ ccopt("-framework Foundation")
    @ cclib("-lstdc++")
  | _ => []
  };

let flags_with_sanitize =
  switch (get_os) {
  | Linux => flags @ ccopt("-fsanitize=address")
  | _ => flags
  };

let cxx_flags =
  switch (get_os) {
  | Linux => c_flags @ ["-fPIC", "-lstdc++"]
  | Mac => c_flags @ ["-x", "objective-c++", "-lstdc++"]
  | Windows => c_flags @ ["-fno-exceptions", "-fno-rtti", "-lstdc++"]
  | _ => c_flags
  };

Configurator.V1.Flags.write_sexp("c_flags.sexp", c_flags);
Configurator.V1.Flags.write_sexp("cxx_flags.sexp", cxx_flags);
Configurator.V1.Flags.write_sexp("flags.sexp", flags);
Configurator.V1.Flags.write_sexp(
  "flags_with_sanitize.sexp",
  flags_with_sanitize,
);
