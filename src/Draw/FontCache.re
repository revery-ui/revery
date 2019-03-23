open Revery_Core;

type fontInfo = (string, int);
type t = Hashtbl.t(fontInfo, Fontkit.fk_face);

type fontLoaded = Event.t(unit);
let onFontLoaded: fontLoaded = Event.create();

let _cache: t = Hashtbl.create(100);
let _loadingCache = Hashtbl.create(10);
let _isSome = a =>
  switch (a) {
  | Some(_) => true
  | None => false
  };

let load = (fontName: string, size: int) => {
  let execDir = Revery_Core.Environment.getExecutingDirectory();
  switch (Hashtbl.find_opt(_cache, (fontName, size))) {
  | Some(fk) => fk
  | None =>
    let fontKey = (fontName, size);
    let isLoading = _isSome(Hashtbl.find_opt(_loadingCache, fontKey));
    if (!isLoading) {
      Hashtbl.add(_loadingCache, fontKey, true);
      let success = fk => {
        Hashtbl.remove(_loadingCache, fontKey);
        Hashtbl.add(_cache, fontKey, fk);
        Event.dispatch(onFontLoaded, ());
        Lwt.return();
      };
      let _ = Lwt.bind(Fontkit.load(execDir ++ fontName, size), success);
      ();
    };
    Fontkit.dummyFont(size);
  };
};
