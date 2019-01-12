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
  /* TODO: Support url paths? */
  let execDir = Environment.getExecutingDirectory();
  let relativeImagePath = execDir ++ imagePath;

  let cacheResult = Hashtbl.find_opt(_cache, relativeImagePath);

  let ret =
    switch (cacheResult) {
    | Some(r) => r
    | None =>
      let initialImage = Image.fromColor(255, 0, 0, 255);
      let initialPixels = Image.getPixels(initialImage);

      /* Create an initial texture container */
      let texture = glCreateTexture();
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        initialPixels,
      );

      let imageLoadPromise = Image.load(relativeImagePath);

      let success = img => {
        let pixels = Image.getPixels(img);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
          GL_TEXTURE_2D,
          0,
          GL_RGBA,
          GL_RGBA,
          GL_UNSIGNED_BYTE,
          pixels,
        );
        Lwt.return();
      };

      let _ = Lwt.bind(imageLoadPromise, success);
      Hashtbl.add(_cache, relativeImagePath, texture);
      texture;
    };

  ret;
};