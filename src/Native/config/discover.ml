module C = Configurator.V1

type os = Windows | Mac | Linux | Unknown

type config = {libs: string list; cflags: string list; flags: string list}

let get_mac_config () =
  {cflags= ["-I"; "."; "-x"; "objective-c"]; libs= []; flags= []}

let get_linux_config c =
  let default = {libs= []; cflags= []; flags= []} in
  match C.Pkg_config.get c with
  | None -> default
  | Some pc -> (
    match C.Pkg_config.query pc ~package:"gtk+-3.0" with
    | None -> default
    | Some conf -> {libs= conf.libs; cflags= conf.cflags; flags= []} )

let uname () =
  let ic = Unix.open_process_in "uname" in
  let uname = input_line ic in
  let () = close_in ic in
  uname

let get_os =
  match Sys.os_type with
  | "Win32" -> Windows
  | _ -> (
    match uname () with "Darwin" -> Mac | "Linux" -> Linux | _ -> Unknown )

let () =
  C.main ~name:"discover" (fun c ->
      let conf =
        match get_os with
        | Mac -> get_mac_config ()
        | Linux -> get_linux_config c
        | _ -> {libs= []; flags= []; cflags= []}
      in
      C.Flags.write_sexp "flags.sexp" conf.flags ;
      C.Flags.write_sexp "c_flags.sexp" conf.cflags ;
      C.Flags.write_sexp "c_library_flags.sexp" conf.libs )
