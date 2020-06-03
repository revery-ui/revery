open Configurator.V1.Flags;

let getenv = name =>
  try(Sys.getenv(name)) {
  | Not_found => failwith("Error: Undefined environment variable: " ++ name)
  };

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

let () = {
  let ccopt = s => ["-ccopt", s];
  let cclib = s => ["-cclib", s];
  let framework = s => ["-framework", s];
  let flags =
    switch (get_os) {
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

    | _ => []
    };

  let cflags =
    switch (get_os) {
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

    | _ => failwith("cflags: unknown platform")
    };

  let libs =
    switch (get_os) {
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

    | _ => failwith("libs: Unknown platform")
    };

  write_sexp("flags.sexp", flags);
  write_lines("c_flags.txt", cflags);
  write_sexp("c_flags.sexp", cflags);
  write_sexp("c_library_flags.sexp", libs);
  write_lines("c_library_flags.txt", libs);
  write_sexp(
    "cclib_c_library_flags.sexp",
    libs |> List.map(o => ["-cclib", o]) |> List.flatten,
  );
};
