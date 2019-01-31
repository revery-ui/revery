module C = Configurator.V1

let f elem = Printf.printf "I'm looking at element %s now\n" elem

let () = 
  C.main ~name:"linux" (fun c ->
    let default : C.Pkg_config.package_conf =
      { libs   = []
      ; cflags = []
      }
    in
    let conf  = match C.Pkg_config.get c with
    | None -> default
    | Some pc ->
        match (C.Pkg_config.query pc ~package:"gtk+-3.0") with
        | None -> default
        | Some conf -> conf
    in

    C.Flags.write_sexp "flags.sexp" [];
    C.Flags.write_sexp "c_flags.sexp" conf.cflags;
    C.Flags.write_sexp "c_library_flags.sexp" conf.libs)

  (**
type os =
  | Windows 
  | Mac 
  | Linux 
  | Unknown 
let uname () =
  let ic = Unix.open_process_in "uname" in
  let uname = input_line ic in let () = close_in ic in uname
let get_os =
  match Sys.os_type with
  | "Win32" -> Windows
  | _ ->
      (match uname () with
       | "Darwin" -> Mac
       | "Linux" -> Linux
       | _ -> Unknown)
let c_flags =
  match get_os with | Mac -> ["-I"; "."; "-x"; "objective-c"] | Linux -> 
    ["`pkg-config --cflags gtk+-3.0`"; "`pkg-config --libs gtk+-3.0`"] | _ -> []
let flags = []
;;

let write_sexp fn eles =
   C.Flags.write_sexp fn eles
in

C.Flags.write_sexp "c_flags.sexp" c_flags;
C.Flags.write_sexp "flags.sexp" flags
*)
