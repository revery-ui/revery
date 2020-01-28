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

type fontInfo = (string, int);

type fontLoaded = Event.t(unit);
let onFontLoaded: fontLoaded = Event.create();

type t = (Harfbuzz.hb_face, Skia.Typeface.t);

let _cache: InternalCache.t(t) = InternalCache.create();
let _loadingCache: InternalCache.t(bool) = InternalCache.create();
let _isSome = a =>
  switch (a) {
  | Some(_) => true
  | None => false
  };

let load: (string) => result(t, string) =
  (fontName: string) => {
    let assetPath = Environment.getAssetPath(fontName);

    // TODO: Cache
    let skiaTypeface = Skia.Typeface.makeFromFile(assetPath, 0);
    let harfbuzzFace = Harfbuzz.hb_new_face(assetPath);

    switch ((skiaTypeface, harfbuzzFace)) {
    | (Some(skia), Ok(hb)) => 
      Event.dispatch(onFontLoaded, ());
      Ok((hb, skia))
    | (_, Error(msg)) => 
      Error("Error loading typeface: " ++ msg);
    | (None, _) => 
      Error("Error loading typeface.");
    }

  };
