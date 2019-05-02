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

let _cache: InternalCache.t(Fontkit.fk_face) = InternalCache.create();
let _loadingCache: InternalCache.t(bool) = InternalCache.create();
let _isSome = a =>
  switch (a) {
  | Some(_) => true
  | None => false
  };

let load = (fontName: string, size: int) => {
  let execDir = Revery_Core.Environment.getExecutingDirectory();
  switch (InternalCache.find_opt(_cache, fontName, size)) {
  | Some(fk) => fk
  | None =>
    let isLoading =
      _isSome(InternalCache.find_opt(_loadingCache, fontName, size));
    if (!isLoading) {
      InternalCache.add(_loadingCache, fontName, size, true);
      let success = fk => {
        InternalCache.remove(_loadingCache, fontName, size);
        InternalCache.add(_cache, fontName, size, fk);
        Event.dispatch(onFontLoaded, ());
        Lwt.return();
      };
      let _ = Lwt.bind(Fontkit.load(execDir ++ fontName, size), success);
      ();
    };
    Fontkit.dummyFont(size);
  };
};
