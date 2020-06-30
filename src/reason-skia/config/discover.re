module Configurator = Configurator.V1;

let getenv = name =>
  try(Sys.getenv(name)) {
  | Not_found => failwith("Error: Undefined environment variable: " ++ name)
  };

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

let ccopt = s => ["-ccopt", s];
let cclib = s => ["-cclib", s];
let framework = s => ["-framework", s];
let flags = os =>
  switch (os) {
  | Linux =>
    []
    @ ["-verbose"]
    @ cclib("-lfontconfig")
    @ cclib("-lfreetype")
    @ cclib("-lz")
    @ cclib("-lbz2")
    @ cclib("-lskia")
    @ cclib("-lSDL2")
    @ ccopt("-L" ++ getenv("FREETYPE2_LIB_PATH"))
    @ ccopt("-L" ++ getenv("SDL2_LIB_PATH"))
    @ ccopt("-L" ++ getenv("SKIA_LIB_PATH"))
    @ ccopt("-L" ++ getenv("JPEG_LIB_PATH"))
    @ ccopt("-I" ++ getenv("FREETYPE2_INCLUDE_PATH"))
    @ ccopt("-I" ++ getenv("SKIA_INCLUDE_PATH"))
    @ cclib("-ljpeg")
    @ ccopt("-I/usr/include")
    @ ccopt("-lstdc++")
  | Windows =>
    []
    @ cclib("-lskia")
    @ cclib("-lSDL2")
    @ ccopt("-L" ++ getenv("SDL2_LIB_PATH"))
    @ ccopt("-L" ++ getenv("SKIA_LIB_PATH"))
  | Mac => []
  };

let cflags = os =>
  switch (os) {
  | Mac =>
    []
    @ ["-I" ++ getenv("SDL2_INCLUDE_PATH")]
    @ ["-I" ++ getenv("SKIA_INCLUDE_PATH")]
    @ ["-I" ++ getenv("SKIA_INCLUDE_PATH") ++ "/c"]
  | Linux =>
    []
    @ ["-lSDL2"]
    @ ["-lskia"]
    @ ["-I" ++ getenv("SDL2_INCLUDE_PATH")]
    @ ["-I" ++ getenv("SKIA_INCLUDE_PATH")]
    @ ["-I" ++ getenv("SKIA_INCLUDE_PATH") ++ "/c"]
    @ ["-L" ++ getenv("SKIA_LIB_PATH")]
    @ ["-L" ++ getenv("SDL2_LIB_PATH")]
    @ ["-L" ++ getenv("JPEG_LIB_PATH")]
    @ ["-lstdc++"]
    @ ["-ljpeg"]
  | Windows =>
    []
    @ ["-std=c++1y"]
    @ ["-I" ++ getenv("SDL2_INCLUDE_PATH")]
    @ ["-I" ++ getenv("SKIA_INCLUDE_PATH")]
    @ ["-I" ++ getenv("SKIA_INCLUDE_PATH") ++ "/c"]
  };

let libs = os =>
  switch (os) {
  | Mac =>
    []
    @ ["-L" ++ getenv("JPEG_LIB_PATH")]
    @ ["-L" ++ getenv("SKIA_LIB_PATH")]
    @ ["-L" ++ getenv("FREETYPE2_LIB_PATH")]
    @ ["-L" ++ getenv("SDL2_LIB_PATH")]
    @ framework("Carbon")
    @ framework("Cocoa")
    @ framework("CoreFoundation")
    @ framework("CoreAudio")
    @ framework("CoreVideo")
    @ framework("CoreServices")
    @ framework("CoreGraphics")
    @ framework("CoreText")
    @ framework("CoreFoundation")
    @ framework("AudioToolbox")
    @ framework("ForceFeedback")
    @ framework("IOKit")
    @ framework("Metal")
    @ ["-liconv"]
    @ ["-lSDL2"]
    @ ["-lskia"]
    @ ["-lstdc++"]
    @ [getenv("JPEG_LIB_PATH") ++ "/libturbojpeg.a"]
  | Linux =>
    []
    @ [
      "-lSDL2",
      "-lskia",
      "-lfreetype",
      "-lfontconfig",
      "-lz",
      "-lbz2",
      "-L" ++ getenv("JPEG_LIB_PATH"),
      "-ljpeg",
      "-lpthread",
      "-lstdc++",
      "-L" ++ getenv("SDL2_LIB_PATH"),
      "-L" ++ getenv("SKIA_LIB_PATH"),
      "-L" ++ getenv("FREETYPE2_LIB_PATH"),
    ]
  | Windows =>
    []
    @ ["-luser32"]
    @ ["-lskia"]
    @ ["-lSDL2"]
    @ ["-lstdc++"]
    @ ["-L" ++ getenv("SDL2_LIB_PATH")]
    @ ["-L" ++ getenv("SKIA_LIB_PATH")]
  };

Configurator.main(~name="reason-sdl2", conf => {
  let os = get_os(conf);
  Configurator.Flags.write_sexp("flags.sexp", flags(os));
  Configurator.Flags.write_lines("c_flags.txt", cflags(os));
  Configurator.Flags.write_sexp("c_flags.sexp", cflags(os));
  Configurator.Flags.write_sexp("c_library_flags.sexp", libs(os));
  Configurator.Flags.write_lines("c_library_flags.txt", libs(os));
  Configurator.Flags.write_sexp(
    "cclib_c_library_flags.sexp",
    libs(os) |> List.map(o => ["-cclib", o]) |> List.flatten,
  );
});
