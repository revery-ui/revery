module Configurator = Configurator.V1;

type os =
  | Windows
  | Mac
  | Linux;

let detect_system_header = {|
  #if __APPLE__
    #define PLATFORM_NAME "mac"
  #elif __linux__
    #define PLATFORM_NAME "linux"
  #elif WIN32
    #define PLATFORM_NAME "windows"
  #endif
|};

let get_os = t => {
  let header = {
    let file = Filename.temp_file("discover", "os.h");
    let fd = open_out(file);
    output_string(fd, detect_system_header);
    close_out(fd);
    file;
  };
  let platform =
    Configurator.C_define.import(
      t,
      ~includes=[header],
      [("PLATFORM_NAME", String)],
    );
  switch (platform) {
  | [(_, String("linux"))] => Linux
  | [(_, String("mac"))] => Mac
  | [(_, String("windows"))] => Windows
  | _ => failwith("Unknown operating system")
  };
};

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
  switch (Configurator.Pkg_config.get(c)) {
  | None => default
  | Some(pc) =>
    switch (Configurator.Pkg_config.query(pc, ~package="gtk+-3.0")) {
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

Configurator.main(~name="discover", conf => {
  let os = get_os(conf);
  let conf =
    switch (os) {
    | Mac => get_mac_config()
    | Linux => get_linux_config(conf)
    | Windows => get_win32_config()
    };

  Configurator.Flags.write_sexp("flags.sexp", conf.flags);
  Configurator.Flags.write_sexp("c_flags.sexp", conf.cflags);
  Configurator.Flags.write_sexp("c_library_flags.sexp", conf.libs);
});
