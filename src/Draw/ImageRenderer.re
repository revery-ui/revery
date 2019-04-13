open Reglfw.Glfw;
open Revery_Core;

module Image = Reglfw.Image;

type t = {
  mutable hasLoaded: bool,
  texture,
  mutable width: int,
  mutable height: int,
};

let initialPixels =
  Lazy.make(() => {
    let initialImage = Image.fromColor(255, 0, 0, 255);
    Image.getPixels(initialImage);
  });

type cache = Hashtbl.t(string, t);
let _cache: cache = Hashtbl.create(100);

let getTexture = (imagePath: string) => {
  /* TODO: Support url paths? */
  let execDir = Environment.getExecutingDirectory();
  let relativeImagePath = execDir ++ imagePath;

  let cacheResult = Hashtbl.find_opt(_cache, relativeImagePath);

  switch (cacheResult) {
  | Some(r) => r
  | None =>
    /* Create an initial texture container */
    let texture = glCreateTexture();
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    let initialPixels = initialPixels();
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGBA,
      GL_RGBA,
      GL_UNSIGNED_BYTE,
      initialPixels,
    );

    let imageLoadPromise = Image.load(relativeImagePath);

    let ret: t = {hasLoaded: false, texture, width: 1, height: 1};

    let success = img => {
      let pixels = Image.getPixels(img);
      let {width, height, _}: Image.dimensions = Image.getDimensions(img);
      glBindTexture(GL_TEXTURE_2D, texture);
      glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        pixels,
      );
      ret.hasLoaded = true;
      ret.width = width;
      ret.height = height;
      Lwt.return();
    };

    let _ = Lwt.bind(imageLoadPromise, success);
    Hashtbl.replace(_cache, relativeImagePath, ret);
    ret;
  };
};
