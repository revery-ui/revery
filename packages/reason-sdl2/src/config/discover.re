module Configurator = Configurator.V1;

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

let root = Sys.getenv("cur__root");
let c_flags = [
  "-I",
  Sys.getenv("SDL2_INCLUDE_PATH"),
  "-I",
  Filename.concat(root, "include"),
  "-I",
  Filename.concat(root, "src"),
];

let c_flags = os =>
  switch (os) {
  | Android
  | IOS
  | Mac => c_flags
  | Linux => c_flags @ ["-fPIC"]
  | Windows => c_flags @ ["-mwindows"]
  };

let libFolderPath = Sys.getenv("SDL2_LIB_PATH");
let libFilePath = libFolderPath ++ "/libSDL2.a";
prerr_endline("SDL2 Library Folder Path: " ++ libFolderPath);

let ccopt = s => ["-ccopt", s];
let cclib = s => ["-cclib", s];

let flags = os =>
  switch (os) {
  | Android =>
    []
    @ ccopt(libFilePath)
    @ cclib("-lEGL")
    @ cclib("-lGLESv1_CM")
    @ cclib("-lGLESv2")
  | IOS =>
    []
    @ ccopt(libFilePath)
    @ ccopt("-framework Foundation")
    @ ccopt("-framework OpenGLES")
    @ ccopt("-framework UIKit")
    @ ccopt("-framework IOKit")
    @ ccopt("-framework CoreVideo")
    @ ccopt("-framework CoreAudio")
    @ ccopt("-framework AudioToolbox")
    @ ccopt("-framework Metal")
    @ ccopt("-liconv")
  | Linux =>
    []
    @ cclib("-lGL")
    @ cclib("-lGLU")
    @ ccopt(libFilePath)
    @ cclib("-lX11")
    @ cclib("-lXxf86vm")
    @ cclib("-lXrandr")
    @ cclib("-lXinerama")
    @ cclib("-lXcursor")
    @ cclib("-lpthread")
    @ cclib("-lXi")
  | Mac =>
    []
    @ ccopt(libFilePath)
    @ ccopt("-framework AppKit")
    @ ccopt("-framework Foundation")
    @ ccopt("-framework OpenGL")
    @ ccopt("-framework Cocoa")
    @ ccopt("-framework IOKit")
    @ ccopt("-framework CoreVideo")
    @ ccopt("-framework CoreAudio")
    @ ccopt("-framework AudioToolbox")
    @ ccopt("-framework ForceFeedback")
    @ ccopt("-framework Metal")
    @ ccopt("-framework Carbon")
    @ ccopt("-liconv")
  | Windows =>
    let maybeAngleLibPath = Sys.getenv_opt("ANGLE_LIB_PATH");
    let angleLibPath = switch (maybeAngleLibPath) {
    | None => 
    prerr_endline ("Unable to get libraries for esy-angle-prebuilt")
    failwith("Unable to get libraries for esy-angle-prebuilt)")
    | Some(path) =>
    print_endline ("ANGLE Library Path: " ++ path);
    path
    };
    let eglPath = angleLibPath ++ "/libEGL.a";
    let glesv2Path = angleLibPath ++ "/libGLESv2.a";
    []
    // On Windows, we ship the DLL side-by-side
    @ ccopt("-L" ++ libFolderPath)
    @ cclib("-lSDL2")
    @ ccopt(eglPath)
    @ ccopt(glesv2Path)
    // We use the ANGLE DLLs (to use Direct3D apis instead of OpenGL)
    @ cclib("-lgdi32")
    @ cclib("-subsystem windows")
  };

let c_library_flags = os =>
  switch (os) {
  | Android
  | IOS
  | Mac
  | Linux => [libFilePath]
  | Windows => ["-L" ++ libFolderPath, "-lSDL2"]
  };

let cxx_flags = os =>
  switch (os) {
  | Android
  | Linux => c_flags(os) @ ["-std=c++11"]
  | IOS
  | Mac => c_flags(os) @ ["-x", "objective-c++"]
  | Windows =>
    c_flags(os) @ ["-fno-exceptions", "-fno-rtti", "-lstdc++", "-mwindows"]
  };

Configurator.main(~name="reason-sdl2", conf => {
  let os = get_os(conf);
  Configurator.Flags.write_sexp("c_library_flags.sexp", c_library_flags(os));
  Configurator.Flags.write_sexp("c_flags.sexp", c_flags(os));
  Configurator.Flags.write_sexp("cxx_flags.sexp", cxx_flags(os));
  Configurator.Flags.write_sexp("flags.sexp", flags(os));
});
