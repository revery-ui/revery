type t = (~italic: bool, FontWeight.t) => option(Skia.Typeface.t);

module FontFamilyHashable = {
  type t = {
    familyName: string,
    weight: FontWeight.t,
    italic: bool,
  };

  let equal = (a, b) =>
    String.equal(a.familyName, b.familyName)
    && a.weight == b.weight
    && a.italic == b.italic;

  let hash = Hashtbl.hash;
};

module FontDescriptorWeight = {
  type t = option(Skia.Typeface.t);
  let weight = _ => 1;
};

module FontFamilyCache = Lru.M.Make(FontFamilyHashable, FontDescriptorWeight);

let cache = FontFamilyCache.create(~initialSize=8, 64);

let system = (familyName): t =>
  (~italic, weight) => {
    let fontDescr: FontFamilyHashable.t = {familyName, weight, italic};
    switch (FontFamilyCache.find(fontDescr, cache)) {
    | Some(fd) =>
      FontFamilyCache.promote(fontDescr, cache);
      fd;
    | None =>
      let fd = Discovery.find(~weight, ~italic, familyName);
      FontFamilyCache.add(fontDescr, fd, cache);
      FontFamilyCache.trim(cache);
      fd;
    };
  };

let fromFiles =
    (
      solver: (~weight: FontWeight.t, ~italic: bool) => string,
      ~italic,
      weight,
    ) => {
  let familyName = solver(~italic, ~weight);
  let fontDescr: FontFamilyHashable.t = {familyName, weight, italic};
  switch (FontFamilyCache.find(fontDescr, cache)) {
  | Some(tf) =>
    FontFamilyCache.promote(fontDescr, cache);
    tf;
  | None =>
    let assetPath = Revery_Core.Environment.getAssetPath(familyName);
    let tf = Skia.Typeface.makeFromFile(assetPath, 0);
    FontFamilyCache.add(fontDescr, tf, cache);
    FontFamilyCache.trim(cache);
    tf;
  };
};

let fromFile = (fileName, ~italic as _, _) => {
  let fontDescr: FontFamilyHashable.t = {
    familyName: fileName,
    weight: FontWeight.Normal,
    italic: false,
  };
  switch (FontFamilyCache.find(fontDescr, cache)) {
  | Some(tf) =>
    FontFamilyCache.promote(fontDescr, cache);
    tf;
  | None =>
    let assetPath = Revery_Core.Environment.getAssetPath(fileName);
    let tf = Skia.Typeface.makeFromFile(assetPath, 0);
    FontFamilyCache.add(fontDescr, tf, cache);
    FontFamilyCache.trim(cache);
    tf;
  };
};

let default =
  switch (Revery_Core.Environment.os) {
  | Linux => system("Liberation Sans")
  | Mac(_) => system("System Font")
  | _ => system("Arial")
  };

let defaultMono =
  switch (Revery_Core.Environment.os) {
  | Mac(_) => system("Menlo")
  | Windows => system("Consolas")
  | _ => fromFile("Inconsolata.otf")
  };

let defaultSerif =
  switch (Revery_Core.Environment.os) {
  | Mac(_) => system("Palatino")
  | Linux => system("Liberation Serif")
  | _ => system("Times New Roman")
  };

let resolve = (~italic=false, weight, solver) =>
  solver(~italic, weight) |> FontCache.load;

let toSkia = (~italic=false, weight, solver) => solver(~italic, weight);
