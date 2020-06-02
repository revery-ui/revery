open FontManager;

type t = (FontWeight.t, bool, bool) => string;

let fromFile =
    (
      ~variant:
         option(
           (
             ~fontWeight: FontWeight.t,
             ~italicized: bool,
             ~monospaced: bool,
           ) =>
           string,
         )=?,
      default: string,
    )
    : t =>
  switch (variant) {
  | None => ((_, _, _) => default)
  | Some(solver) => (
      (w, i, m) =>
        switch (w, i, m) {
        | (FontWeight.Normal, false, false) => default
        | _ => solver(~fontWeight=w, ~italicized=i, ~monospaced=m)
        }
    )
  };

module FontFamilyHashable = {
  type t = {
    familyName: string,
    weight: FontWeight.t,
    italicized: bool,
    monospaced: bool,
  };

  let equal = (a, b) =>
    String.equal(a.familyName, b.familyName)
    && a.weight == b.weight
    && a.italicized == b.italicized
    && a.monospaced == b.monospaced;

  let hash = Hashtbl.hash;
};

module FontDescriptorWeight = {
  type t = FontManager.FontDescriptor.t;
  let weight = _ => 1;
};

module FontFamilyCache = Lru.M.Make(FontFamilyHashable, FontDescriptorWeight);

let cache = FontFamilyCache.create(~initialSize=8, 64);

let system = (familyName): t =>
  (weight, italicized, monospaced) => {
    let fontDescr: FontFamilyHashable.t = {
      familyName,
      weight,
      italicized,
      monospaced,
    };
    switch (FontFamilyCache.find(fontDescr, cache)) {
    | Some(fd) =>
      FontFamilyCache.promote(fontDescr, cache);
      fd.path;
    | None =>
      let fd =
        Discovery.find(
          ~weight,
          ~mono=monospaced,
          ~italic=italicized,
          familyName,
        );
      FontFamilyCache.add(fontDescr, fd, cache);
      FontFamilyCache.trim(cache);
      fd.path;
    };
  };

let default =
  switch (Revery_Core.Environment.os) {
  | Linux => system("Liberation Sans")
  | Mac => system("System Font")
  | _ => system("Arial")
  };

let resolve =
    (fontFamily: t, weight: FontWeight.t, italicized: bool, monospaced: bool) =>
  fontFamily(weight, italicized, monospaced) |> FontCache.load;

let toPath =
    (fontFamily: t, weight: FontWeight.t, italicized: bool, monospaced: bool) =>
  fontFamily(weight, italicized, monospaced);
