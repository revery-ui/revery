open Revery_Core;

module StringHash =
  Hashtbl.Make({
    type t = string;
    let equal = String.equal;
    let hash = String.length;
  });

module InternalCache = {
  type t('a) = StringHash.t(Hashtbl.t(int, 'a));

  let create: unit => t('a) =
    () => {
      StringHash.create(10);
    };

  let _getOrCreateSizeDict = (v: t('a), fontName) => {
    switch (StringHash.find_opt(v, fontName)) {
    | Some(x) => x
    | None =>
      let result = Hashtbl.create(10);
      StringHash.add(v, fontName, result);
      result;
    };
  };

  let find_opt = (v: t('a), fontName, size) => {
    let sizeDictionary = _getOrCreateSizeDict(v, fontName);
    switch (Hashtbl.find_opt(sizeDictionary, size)) {
    | Some(v) => Some(v)
    | None => None
    };
  };

  let add = (v: t('a), fontName, size, b) => {
    let sizeDictionary = _getOrCreateSizeDict(v, fontName);
    Hashtbl.add(sizeDictionary, size, b);
  };

  let remove = (v: t('a), fontName, size) => {
    let sizeDictionary = _getOrCreateSizeDict(v, fontName);
    Hashtbl.remove(sizeDictionary, size);
  };
};

type fontLoaded = Event.t(unit);
let onFontLoaded: fontLoaded = Event.create();

type t = {
  hbFace: Harfbuzz.hb_face,
  skiaTypeface: Skia.Typeface.t,
}

let _cache: InternalCache.t(t) = InternalCache.create();

let load: (string) => option(t) =
  (fontName: string) => {
    let size = 12;
    let assetPath = Environment.getAssetPath(fontName);
    switch (InternalCache.find_opt(_cache, fontName, size)) {
    | Some(v) => 
      prerr_endline ("Got font from cache: " ++ fontName);
      Some(v)
    | None =>
          prerr_endline ("Loading font from: " ++ assetPath);
          let hbFace = Harfbuzz.hb_new_face(assetPath);

          switch (hbFace) {
          | Ok(v) => 
            let skiaTypeface = Skia.Typeface.makeFromFile(assetPath, 0);
            let font = { hbFace: v, skiaTypeface };
            InternalCache.add(_cache, fontName, size, font);
            Event.dispatch(onFontLoaded, ());
            Some(font)
          | _ => None;
          };
    };
  };
