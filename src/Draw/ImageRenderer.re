open Revery_Core;

module Log = (val Log.withNamespace("Revery.ImageRenderer"));

type cache = Hashtbl.t(string, option(Skia.Image.t));
let _cache: cache = Hashtbl.create(100);

let getTexture = (imagePath: string) => {
  /* TODO: Support url paths? */
  let imagePath = Environment.getAssetPath(imagePath);
  let cacheResult = Hashtbl.find_opt(_cache, imagePath);

  switch (cacheResult) {
  | Some(r) =>
    switch (r) {
    | Some(_) as v => v
    | None => None
    }
  | None =>
    Log.info("Loading from path: " ++ imagePath);

    let maybeData = imagePath |> Skia.Data.makeFromFileName;

    let maybeImage =
      Option.bind(maybeData, data => Skia.Image.makeFromEncoded(data, None));

    Hashtbl.replace(_cache, imagePath, maybeImage);
    maybeImage;
  };
};
