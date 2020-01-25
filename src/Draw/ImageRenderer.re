open Sdl2.Gl;
open Revery_Core;

open Skia;
module Image = Sdl2.Image;

module Log = (val Log.withNamespace("Revery.ImageRenderer"));

let initialPixels =
  Lazy.make(() => {
    let initialImage = Image.fromColor(255, 0, 0, 255);
    Image.getPixels(initialImage);
  });

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
    //let data = Skia.Data.makeFromFileName(imagePath);
    let data = Skia.Data.makeFromFileName(imagePath);
    Log.info("Got data.");
    let img = Skia.Image.makeFromEncoded(data, None);
    Log.info("Got image.");

    Hashtbl.replace(_cache, imagePath, img);
    img;
  };
};
