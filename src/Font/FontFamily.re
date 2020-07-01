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
    let tf = Skia.Typeface.makeFromFile(familyName, 0);
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
    let tf = Skia.Typeface.makeFromFile(fileName, 0);
    FontFamilyCache.add(fontDescr, tf, cache);
    FontFamilyCache.trim(cache);
    tf;
  };
};

let default =
  switch (Revery_Core.Environment.os) {
  | Linux => system("Liberation Sans")
  | _ => system("Arial")
  };

let resolve = (~italic=false, weight, solver) =>
  solver(~italic, weight) |> FontCache.load;

let toMaybeSkia = (~italic=false, weight, solver) => solver(~italic, weight);
