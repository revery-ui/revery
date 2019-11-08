open Sdl2.Gl;
open Revery_Core;

module Image = Sdl2.Image;

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

let normaliseUrl = (~text, ~extension) =>
  Str.(global_replace(regexp("[^a-zA-Z0-9_]"), "", text))
  |> (text => text ++ extension)
  |> Fpath.v;

let toFileExtension =
  fun
  | Some(mediaType) =>
    switch (mediaType) {
    | "image/apng" => ".apng"
    | "image/bmp" => ".bmp"
    | "image/gif" => ".gif"
    | "image/x-icon" => ".ico"
    | "image/jpeg" => ".jpg"
    | "image/png" => ".png"
    | "image/svg+xml" => ".svg"
    | "image/tiff" => ".tif"
    | "image/webp" => ".webp"
    | _ => ".png"
    }
  | _ => ".png";

let getTexture = (imagePath: string) => {
  /* TODO: Support url paths? */
  let cacheResult = Hashtbl.find_opt(_cache, imagePath);

  switch (cacheResult) {
  | Some(r) => r
  | None =>
    /* check if path has http or https */
    let isRemote = imagePath |> Uri.of_string |> Uri.scheme |> Option.is_some;

    let imageLoadPromise =
      if (isRemote) {
        /* TODO: what happens if we don't get a response */
        let%lwt image =
          Cohttp_lwt_unix.Client.get(Uri.of_string(imagePath))
          |> Lwt.map(((resp, body)) => {
               /* TODO: this only uses the CWD, make this configurable or default to sys temp? */
               let cwd = Sys.getcwd() |> Fpath.v;
               let fileExtension =
                 Cohttp.(Header.get_media_type(resp |> Response.headers))
                 |> toFileExtension;
               let normalisedImagePath =
                 Fpath.append(
                   cwd,
                   normaliseUrl(~text=imagePath, ~extension=fileExtension),
                 );

               let%lwt image =
                 body
                 |> Cohttp_lwt.Body.to_string
                 |> Lwt.map(body => {
                      switch (Bos.OS.File.write(normalisedImagePath, body)) {
                      | Ok(v) =>
                        Image.load(normalisedImagePath |> Fpath.to_string)
                      | Error(`Msg(msg)) =>
                        /* TODO: handle this */
                        Image.load(normalisedImagePath |> Fpath.to_string)
                      }
                    });

               /* TODO: handle error */
               Bos.OS.File.delete(~must_exist=true, normalisedImagePath)
               |> ignore;

               image;
             });

        image;
      } else {
        Image.load(Environment.getAssetPath(imagePath));
      };

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
    Hashtbl.replace(_cache, imagePath, ret);
    ret;
  };
};
