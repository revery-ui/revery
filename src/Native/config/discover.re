open Configurator.V1;
open C_define;
open Flags;

type os =
  | Android
  | IOS
  | Linux
  | Mac
  | Windows;

let detect_system_header = {|
  #if __APPLE__
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE
      #define PLATFORM_NAME "ios"
    #else
      #define PLATFORM_NAME "mac"
    #endif
  #elif __linux__
    #if __ANDROID__
      #define PLATFORM_NAME "android"
    #else
      #define PLATFORM_NAME "linux"
    #endif
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
  | [(_, String("android"))] => Android
  | [(_, String("ios"))] => IOS
  | [(_, String("linux"))] => Linux
  | [(_, String("mac"))] => Mac
  | [(_, String("windows"))] => Windows
  | _ => failwith("Unknown operating system")
  };
};

type feature =
  | COCOA
  | GTK
  | UIKIT
  | WIN32;

let gen_config_header = (conf, features) => {
  let includes = value =>
    List.exists((==)(value), features)
      ? Value.Int(1) : Value.Switch(false);
  let os = {
    let os = get_os(conf);
    switch (os) {
    | Android => "android"
    | IOS => "ios"
    | Linux => "linux"
    | Mac => "mac"
    | Windows => "windows"
    };
  };
  let is_os = os => get_os(conf) == os ? Value.Int(1) : Value.Switch(false);

  gen_header_file(
    conf,
    [
      ("PLATFORM_NAME", Value.String(os)),
      ("USE_COCOA", includes(COCOA)),
      ("USE_GTK", includes(GTK)),
      ("USE_UIKIT", includes(UIKIT)),
      ("USE_WIN32", includes(WIN32)),
      ("IS_IOS", is_os(IOS)),
      ("IS_MACOS", is_os(Mac)),
      ("IS_ANDROID", is_os(Android)),
      ("IS_LINUX", is_os(Linux)),
      ("IS_WINDOWS", is_os(Windows)),
    ],
  );
};

type config = {
  features: list(feature),
  libs: list(string),
  cflags: list(string),
  flags: list(string),
};

let ccopt = s => ["-ccopt", s];
let cclib = s => ["-cclib", s];

let get_ios_config = () => {
  features: [UIKIT],
  cflags: ["-I", ".", "-x", "objective-c"],
  libs: [],
  flags: [],
};
let get_mac_config = () => {
  features: [COCOA],
  cflags: ["-I", ".", "-x", "objective-c", "-Wno-deprecated-declarations"],
  libs: [],
  flags: [] @ cclib("-ObjC"),
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

let get_win32_config = () => {
  features: [WIN32],
  cflags: [],
  libs: [],
  flags: [] @ cclib("-luuid") @ cclib("-lole32") @ cclib("-lcomdlg32"),
};

main(~name="discover", t => {
  let os = get_os(t);
  let conf =
    switch (os) {
    | Android
    | Linux => get_linux_config(t)
    | IOS => get_ios_config()
    | Mac => get_mac_config()
    | Windows => get_win32_config()
    };
  gen_config_header(~fname="config.h", t, conf.features);
  write_sexp("flags.sexp", conf.flags);
  write_sexp("c_flags.sexp", conf.cflags);
  write_sexp("c_library_flags.sexp", conf.libs);
});
