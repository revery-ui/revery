open FontManager;

type t = (~italic: bool, ~mono: bool, FontWeight.t) => string;

let fromFiles = (solver, ~italic, ~mono, weight) =>
  solver(~weight, ~italic, ~mono);

let fromFile = (fileName, ~italic as _, ~mono as _, _) => fileName;

module FontFamilyHashable = {
  type t = {
    familyName: string,
    weight: FontWeight.t,
    italic: bool,
    mono: bool,
  };

  let equal = (a, b) =>
    String.equal(a.familyName, b.familyName)
    && a.weight == b.weight
    && a.italic == b.italic
    && a.mono == b.mono;

  let hash = Hashtbl.hash;
};

module FontDescriptorWeight = {
  type t = FontManager.FontDescriptor.t;
  let weight = _ => 1;
};

module FontFamilyCache = Lru.M.Make(FontFamilyHashable, FontDescriptorWeight);

let cache = FontFamilyCache.create(~initialSize=8, 64);

let system = (familyName): t =>
  (~italic, ~mono, weight) => {
    let fontDescr: FontFamilyHashable.t = {familyName, weight, italic, mono};
    switch (FontFamilyCache.find(fontDescr, cache)) {
    | Some(fd) =>
      FontFamilyCache.promote(fontDescr, cache);
      fd.path;
    | None =>
      let fd = Discovery.find(~weight, ~mono, ~italic, familyName);
      FontFamilyCache.add(fontDescr, fd, cache);
      FontFamilyCache.trim(cache);
      fd.path;
    };
  };

let default =
  switch (Revery_Core.Environment.os) {
  | Linux => system("Liberation Sans")
  | _ => system("Arial")
  };

let resolve = (~italic=false, ~mono=false, weight, solver) =>
  solver(~italic, ~mono, weight) |> FontCache.load;

let toPath = (~italic=false, ~mono=false, weight, solver) =>
  solver(~italic, ~mono, weight);
