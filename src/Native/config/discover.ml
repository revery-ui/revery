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
let c_flags = []
let flags = []
;;
Configurator.V1.Flags.write_sexp "c_flags.sexp" c_flags;
Configurator.V1.Flags.write_sexp "flags.sexp" flags;
