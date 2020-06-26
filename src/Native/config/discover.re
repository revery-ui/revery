module C = Configurator.V1;

type os =
  | Windows
  | Mac
  | Linux;

type config = {
  libs: list(string),
  cflags: list(string),
  flags: list(string),
};

let get_mac_config = () => {
  cflags: ["-I", ".", "-x", "objective-c"],
  libs: [],
  flags: [],
};

let get_linux_config = c => {
  let default = {libs: [], cflags: [], flags: []};
  switch (C.Pkg_config.get(c)) {
  | None => default
  | Some(pc) =>
    switch (C.Pkg_config.query(pc, ~package="gtk+-3.0")) {
    | None => default
    | Some(conf) => {libs: conf.libs, cflags: conf.cflags, flags: []}
    }
  };
};

let ccopt = s => ["-ccopt", s];
let cclib = s => ["-cclib", s];

let get_win32_config = () => {
  cflags: [],
  libs: [],
  flags: [] @ cclib("-luuid") @ cclib("-lole32"),
};

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
    | _ => failwith("not supported operating system")
    }
  };

C.main(~name="discover", c => {
  let conf =
    switch (get_os) {
    | Mac => get_mac_config()
    | Linux => get_linux_config(c)
    | Windows => get_win32_config()
    };

  C.Flags.write_sexp("flags.sexp", conf.flags);
  C.Flags.write_sexp("c_flags.sexp", conf.cflags);
  C.Flags.write_sexp("c_library_flags.sexp", conf.libs);
});
