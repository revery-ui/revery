open Sdl2.Gl;
open Revery_Core;
open Revery_Fetch;

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

let cleanUrl = url => Str.(global_replace(regexp("[^a-zA-Z0-9_]"), "", url));

let toFileExtension =
  fun
  | "image/apng" => ".apng"
  | "image/bmp" => ".bmp"
  | "image/gif" => ".gif"
  | "image/x-icon" => ".ico"
  | "image/jpeg" => ".jpg"
  | "image/png" => ".png"
  | "image/svg+xml" => ".svg"
  | "image/tiff" => ".tif"
  | "image/webp" => ".webp"
  | _ => ".png";

let getTexture = (imagePath: string) => {
  /* TODO: Support url paths? */
  let cacheResult = Hashtbl.find_opt(_cache, imagePath);

  switch (cacheResult) {
  | Some(r) => r
  | None =>
    /* will this suffice? */
    let isRemote = imagePath |> Uri.of_string |> Uri.scheme |> Option.is_some;

    let imageLoadPromise =
      if (isRemote) {
        let%lwt maybePathAndImage =
          Fetch.fetch(imagePath)
          |> Lwt.map(
               fun
               | Ok({Fetch.Response.body, headers, _}) => {
                   let fileExtension =
                     headers
                     |> Fetch.Headers.get(~key="content-type")
                     |> Option.value(~default="")
                     |> toFileExtension;

                   Fpath.(
                     Some((
                       append(
                         Filename.get_temp_dir_name() |> v,
                         cleanUrl(imagePath) ++ fileExtension |> v,
                       ),
                       Fetch.Response.Body.toString(body),
                     ))
                   );
                 }
               | Error(_) => None,
             );

        let image =
          switch (maybePathAndImage) {
          | Some((normalisedImagePath, body)) =>
            switch (Bos.OS.File.write(normalisedImagePath, body)) {
            | Ok(_unit) => Image.load(normalisedImagePath |> Fpath.to_string)
            | Error(`Msg(_msg)) =>
              /* TODO: handle this */
              Image.load(Environment.getAssetPath(imagePath))
            }
          | None => Image.load(Environment.getAssetPath(imagePath))
          };

        /* TODO: handle error */
        switch (maybePathAndImage) {
        | Some((path, _image)) =>
          Bos.OS.File.delete(~must_exist=true, path) |> ignore
        | None => ()
        };

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
