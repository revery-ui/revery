module Configurator = Configurator.V1;

let getenv = name =>
  try(Sys.getenv(name)) {
  | Not_found => failwith("Error: Undefined environment variable: " ++ name)
  };

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

let sdl2FilePath = Sys.getenv("SDL2_LIB_PATH") ++ "/libSDL2.a";

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
  | [(_, String("android"))] => Android
  | [(_, String("ios"))] => IOS
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
  | Android =>
    []
    @ ["-verbose"]
    @ cclib("-lfreetype")
    @ cclib("-lz")
    @ cclib("-lskia")
    @ cclib("-lskiasvg")
    @ cclib("-lGLESv2")
    @ cclib("-lGLESv1_CM")
    @ cclib("-lm")
    @ cclib("-llog")
    @ cclib("-landroid")
    @ ccopt("-L" ++ getenv("FREETYPE2_LIB_PATH"))
    @ ccopt("-L" ++ getenv("SDL2_LIB_PATH"))
    @ ccopt("-L" ++ getenv("SKIA_LIB_PATH"))
    @ ccopt("-L" ++ getenv("JPEG_LIB_PATH"))
    @ ccopt("-I" ++ getenv("FREETYPE2_INCLUDE_PATH"))
    @ ccopt("-I" ++ getenv("SKIA_INCLUDE_PATH"))
    @ cclib("-ljpeg")
    @ ccopt("-I/usr/include")
    @ ccopt("-lstdc++")
  | IOS
  | Mac => []
  | Linux =>
    []
    @ ["-verbose"]
    @ cclib("-lfontconfig")
    @ cclib("-lfreetype")
    @ cclib("-lz")
    @ cclib("-lbz2")
    @ cclib("-lskia")
    @ cclib("-lskiasvg")
    @ cclib(sdl2FilePath)
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
  };

let skiaIncludeFlags = {
  let skiaIncludePath = getenv("SKIA_INCLUDE_PATH");
  let skiaLibPath = getenv("SKIA_LIB_PATH");

  let _ = Sys.command("ranlib " ++ skiaLibPath ++ "/libskia.a");

  Sys.readdir(skiaIncludePath)
  |> Array.map(path => "-I" ++ skiaIncludePath ++ "/" ++ path)
  |> Array.append([|"-I" ++ skiaIncludePath|])
  |> Array.to_list;
};
let cflags = os => {
  switch (os) {
  | Android =>
    []
    @ [sdl2FilePath]
    @ ["-lGLESv2"]
    @ ["-lGLESv1_CM"]
    @ ["-lm"]
    @ ["-llog"]
    @ ["-landroid"]
    @ ["-lskia"]
    @ ["-lskiasvg"]
    @ ["-I" ++ getenv("SDL2_INCLUDE_PATH")]
    @ skiaIncludeFlags
    @ ["-L" ++ getenv("SKIA_LIB_PATH")]
    @ ["-L" ++ getenv("SDL2_LIB_PATH")]
    @ ["-L" ++ getenv("JPEG_LIB_PATH")]
    @ ["-lstdc++"]
    @ ["-ljpeg"]
  | Linux =>
    []
    @ [sdl2FilePath]
    @ ["-lskia"]
    @ ["-lskiasvg"]
    @ ["-I" ++ getenv("SDL2_INCLUDE_PATH")]
    @ skiaIncludeFlags
    @ ["-L" ++ getenv("SKIA_LIB_PATH")]
    @ ["-L" ++ getenv("SDL2_LIB_PATH")]
    @ ["-L" ++ getenv("JPEG_LIB_PATH")]
    @ ["-lstdc++"]
    @ ["-ljpeg"]
  | IOS
  | Mac => [] @ ["-I" ++ getenv("SDL2_INCLUDE_PATH")] @ skiaIncludeFlags
  | Windows =>
    []
    @ ["-std=c++1y"]
    @ ["-I" ++ getenv("SDL2_INCLUDE_PATH")]
    @ skiaIncludeFlags
  };
};

let cppflags = os => {
  cflags(os)
  @ {
    switch (os) {
    | IOS
    | Mac => [] @ ["-std=c++14"]
    | _ => []
    };
  };
};

let libs = os =>
  switch (os) {
  | Android =>
    []
    @ [
      "-L" ++ getenv("SDL2_LIB_PATH"),
      "-L" ++ getenv("SKIA_LIB_PATH"),
      "-L" ++ getenv("FREETYPE2_LIB_PATH"),
      sdl2FilePath,
      "-lGLESv2",
      "-lGLESv1_CM",
      "-lm",
      "-llog",
      "-landroid",
      "-lskia",
      "-lskiasvg",
      "-lfreetype",
      "-lz",
      "-L" ++ getenv("JPEG_LIB_PATH"),
      "-ljpeg",
      "-lstdc++",
    ]
  | IOS =>
    []
    @ ["-L" ++ getenv("JPEG_LIB_PATH")]
    @ ["-L" ++ getenv("SKIA_LIB_PATH")]
    @ ["-L" ++ getenv("FREETYPE2_LIB_PATH")]
    @ ["-L" ++ getenv("SDL2_LIB_PATH")]
    @ framework("OpenGLES")
    @ framework("UIKit")
    @ framework("Foundation")
    @ framework("GameController")
    @ framework("AVFoundation")
    @ framework("QuartzCore")
    @ framework("CoreMotion")
    @ framework("CoreFoundation")
    @ framework("CoreAudio")
    @ framework("CoreVideo")
    @ framework("CoreServices")
    @ framework("CoreGraphics")
    @ framework("CoreText")
    @ framework("CoreFoundation")
    @ framework("AudioToolbox")
    @ framework("IOKit")
    @ framework("Metal")
    @ ["-liconv"]
    @ [sdl2FilePath]
    @ ["-lskia"]
    @ ["-lskiasvg"]
    @ ["-lstdc++"]
    @ [getenv("JPEG_LIB_PATH") ++ "/libturbojpeg.a"]
  | Linux =>
    []
    @ [
      "-L" ++ getenv("SDL2_LIB_PATH"),
      "-L" ++ getenv("SKIA_LIB_PATH"),
      "-L" ++ getenv("FREETYPE2_LIB_PATH"),
      sdl2FilePath,
      "-lskia",
      "-lskiasvg",
      "-lfreetype",
      "-lfontconfig",
      "-lz",
      "-lbz2",
      "-L" ++ getenv("JPEG_LIB_PATH"),
      "-ljpeg",
      "-lpthread",
      "-lstdc++",
    ]
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
    @ [sdl2FilePath]
    @ ["-lskia"]
    @ ["-lskiasvg"]
    @ ["-lstdc++"]
    @ [getenv("JPEG_LIB_PATH") ++ "/libturbojpeg.a"]
  | Windows =>
    []
    @ ["-L" ++ getenv("SDL2_LIB_PATH")]
    @ ["-L" ++ getenv("SKIA_LIB_PATH")]
    @ ["-luser32"]
    @ ["-lskia"]
    @ ["-lSDL2"]
    @ ["-lstdc++"]
  };

Configurator.main(~name="reason-sdl2", conf => {
  let os = get_os(conf);
  Configurator.Flags.write_sexp("flags.sexp", flags(os));
  Configurator.Flags.write_lines("c_flags.txt", cflags(os));
  Configurator.Flags.write_sexp("c_flags.sexp", cflags(os));
  Configurator.Flags.write_lines("cpp_flags.txt", cppflags(os));
  Configurator.Flags.write_sexp("cpp_flags.sexp", cppflags(os));
  Configurator.Flags.write_sexp("c_library_flags.sexp", libs(os));
  Configurator.Flags.write_lines("c_library_flags.txt", libs(os));
  Configurator.Flags.write_sexp(
    "cclib_c_library_flags.sexp",
    libs(os) |> List.map(o => ["-cclib", o]) |> List.flatten,
  );
});
