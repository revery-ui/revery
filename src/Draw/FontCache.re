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
  hbFace: option(Harfbuzz.hb_face),
  skiaTypeface: Skia.Typeface.t,
}

let _cache: StringHash.t(t) = StringHash.create(16);

let load: (string) => option(t) =
  (fontName: string) => {
    let assetPath = Environment.getAssetPath(fontName);
    //let _assetPath = "/Users/Library/Comic Sans.ttf";
    switch (StringHash.find_opt(_cache, assetPath)) {
    | Some(v) => 
      prerr_endline ("Got font from cache: " ++ assetPath);
      Some(v)
    | None =>
          prerr_endline ("Loading font from: " ++ assetPath);
          let skiaTypeface = Skia.Typeface.makeFromFile(assetPath, 0);
          //let hbFace = Harfbuzz.hb_new_face(assetPath);

          /*switch (hbFace) {
          | Ok(v) => */
          let font = { hbFace: None, skiaTypeface };
          StringHash.add(_cache, assetPath, font);
          Event.dispatch(onFontLoaded, ());
          Some(font);
          //| _ => None;
          //};
  };
};
