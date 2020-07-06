open Configurator.V1;
open C_define;
open Flags;

type os =
  | Linux
  | Mac
  | Windows;

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
    C_define.import(t, ~includes=[header], [("PLATFORM_NAME", String)]);
  switch (platform) {
  | [(_, String("linux"))] => Linux
  | [(_, String("mac"))] => Mac
  | [(_, String("windows"))] => Windows
  | _ => failwith("Unknown operating system")
  };
};

type feature =
  | GTK
  | COCOA
  | WIN32;

let gen_config_header = (conf, features) => {
  let includes = value =>
    List.exists((==)(value), features)
      ? Value.Int(1) : Value.Switch(false);
  gen_header_file(
    conf,
    [
      ("USE_GTK", includes(GTK)),
      ("USE_COCOA", includes(COCOA)),
      ("USE_WIN32", includes(WIN32)),
    ],
  );
};

type config = {
  features: list(feature),
  libs: list(string),
  cflags: list(string),
  flags: list(string),
};

let get_mac_config = () => {
  features: [COCOA],
  cflags: ["-I", ".", "-x", "objective-c"],
  libs: [],
  flags: [],
};

let get_linux_config = c => {
  let default = {features: [], libs: [], cflags: [], flags: []};
  switch (Pkg_config.get(c)) {
  | None => default
  | Some(pc) =>
    switch (Pkg_config.query(pc, ~package="gtk+-3.0")) {
    | None => default
    | Some(conf) => {
        features: [GTK],
        libs: conf.libs,
        cflags: conf.cflags,
        flags: [],
      }
    }
  };
};

let ccopt = s => ["-ccopt", s];
let cclib = s => ["-cclib", s];

let get_win32_config = () => {
  features: [WIN32],
  cflags: [],
  libs: [],
  flags: [] @ cclib("-luuid") @ cclib("-lole32"),
};

main(~name="discover", t => {
  let os = get_os(t);
  let conf =
    switch (os) {
    | Mac => get_mac_config()
    | Linux => get_linux_config(t)
    | Windows => get_win32_config()
    };
  gen_config_header(~fname="config.h", t, conf.features);
  write_sexp("flags.sexp", conf.flags);
  write_sexp("c_flags.sexp", conf.cflags);
  write_sexp("c_library_flags.sexp", conf.libs);
});
