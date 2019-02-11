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
        let metrics = Fontkit.fk_get_metrics(fk);
  print_endline ("-- name: " ++ fontName);
  print_endline ("-- size: " ++ string_of_int(size));
  print_endline ("-- lineGap: " ++ string_of_int(metrics.height));
  print_endline ("-- ascent: " ++ string_of_int(metrics.ascent));
  print_endline ("-- descent: " ++ string_of_int(metrics.descent));
  print_endline ("-- underlinePosition: " ++ string_of_int(metrics.underlinePosition));
  print_endline ("-- underlineThickness: " ++ string_of_int(metrics.underlineThickness));
  print_endline ("-- unitsPerEm: " ++ string_of_int(metrics.unitsPerEm));
  print_endline ("-- size: " ++ string_of_int(metrics.size));

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
