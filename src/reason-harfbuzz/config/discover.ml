type os =
    | Windows
    | Mac
    | Linux
    | Unknown

let uname () =
    let ic = Unix.open_process_in "uname" in
    let uname = input_line ic in
    let () = close_in ic in
    uname;;

let get_os =
    match Sys.os_type with
    | "Win32" -> Windows
    | _ -> match uname () with
        | "Darwin" -> Mac
        | "Linux" -> Linux
        | _ -> Unknown

let c_flags = ["-fPIC"; "-I"; (Sys.getenv "HARFBUZZ_INCLUDE_PATH")]

let ccopt s = ["-ccopt"; s]
let cclib s = ["-cclib"; s]

let extraFlags =
    match get_os with
    | Linux -> ccopt("-L/usr/lib")
    | _ -> []

let lib_path_flags =
    match get_os with
    | _ -> []
    @ ccopt ("-L" ^ (Sys.getenv "HARFBUZZ_LIB_PATH"))

let flags = []
    @ cclib ("-lharfbuzz")
    @ extraFlags
    @ lib_path_flags
;;

let cxx_flags =
    match get_os with
    | Windows -> c_flags @ ["-fno-exceptions"; "-fno-rtti"; "-lstdc++"]
    | _ -> c_flags
;;

Configurator.V1.Flags.write_sexp "c_flags.sexp" c_flags;
Configurator.V1.Flags.write_sexp "cxx_flags.sexp" cxx_flags;
Configurator.V1.Flags.write_sexp "flags.sexp" flags;
