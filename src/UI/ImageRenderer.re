open Reglfw;
open Reglfw.Glfw;
open Revery_Core;

type t = {
  mutable hasLoaded: bool,
  texture,
  mutable width: int,
  mutable height: int,
};

type cache = Hashtbl.t(string, texture);

let _cache: cache = Hashtbl.create(100);

let getTexture = (imagePath: string) => {
  let cacheResult = Hashtbl.find_opt(_cache, imagePath);

  let ret =
    switch (cacheResult) {
    | Some(r) => r
    | None =>
      /* TODO: Support url paths? */
      let fullImagePath = Environment.getAssetPath(imagePath);
      let initialImage = Image.fromColor(255, 0, 0, 255);

      /* Create an initial texture container */
      let texture = glCreateTexture();
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexImage2D(GL_TEXTURE_2D, initialImage);

      let imageLoadPromise = Image.load(fullImagePath);

      let success = img => {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, img);
        Lwt.return();
      };

      let _ = Lwt.bind(imageLoadPromise, success);
      Hashtbl.add(_cache, imagePath, texture);
      texture;
    };

  ret;
};
