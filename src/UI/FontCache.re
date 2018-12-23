open Revery_Core;
type fontInfo = (string, int);
type t = Hashtbl.t(fontInfo, Fontkit.fk_face);

let _cache: t = Hashtbl.create(100);

let load = (fontName: string, size: int) => {
  let execDir = Environment.getExecutingDirectory();
  let ret: Fontkit.fk_face =
    switch (Hashtbl.find_opt(_cache, (fontName, size))) {
    | Some(fk) => fk
    | None =>
      let fk = Fontkit.load(execDir ++ fontName, size);
      Hashtbl.add(_cache, (fontName, size), fk);
      fk;
    };
  ret;
};
