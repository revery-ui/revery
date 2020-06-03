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
let c_flags = [
  "-I",
  Sys.getenv("SDL2_INCLUDE_PATH"),
  "-I",
  Filename.concat(root, "include"),
  "-I",
  Filename.concat(root, "src"),
];

let c_flags =
  switch (get_os) {
  | Windows => c_flags @ ["-mwindows"]
  | Linux => c_flags @ ["-fPIC"]
  | _ => c_flags
  };

let libFolderPath = Sys.getenv("SDL2_LIB_PATH");
let libFilePath = libFolderPath ++ "/libSDL2.a";
prerr_endline ("SDL2 Library Folder Path: " ++ libFolderPath);

let ccopt = s => ["-ccopt", s];
let cclib = s => ["-cclib", s];

let flags =
  switch (get_os) {
  | Windows =>
    []
    // On Windows, we ship the DLL side-by-side
    @ ccopt("-L" ++ libFolderPath)
    @ cclib("-lSDL2")
    @ cclib("-lgdi32")
    @ cclib("-subsystem windows")
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
  | _ =>
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
  };

let c_library_flags = switch (get_os) {
  | Windows => ["-L" ++ libFolderPath, "-lSDL2"]
  | _ => [libFilePath];
}

let cxx_flags =
  switch (get_os) {
  | Linux => c_flags @ ["-std=c++11"]
  | Windows =>
    c_flags @ ["-fno-exceptions", "-fno-rtti", "-lstdc++", "-mwindows"]
  | Mac => c_flags @ ["-x", "objective-c++"]
  | _ => c_flags
  };

{
  Configurator.V1.Flags.write_sexp("c_library_flags.sexp", c_library_flags);
  Configurator.V1.Flags.write_sexp("c_flags.sexp", c_flags);
  Configurator.V1.Flags.write_sexp("cxx_flags.sexp", cxx_flags);
  Configurator.V1.Flags.write_sexp("flags.sexp", flags);
};
