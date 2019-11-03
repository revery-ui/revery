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

let replaceText = (~replacer, text) =>
  Str.(global_replace(regexp(text), replacer));

let normaliseUrl = text =>
  text
  |> String.escaped
  |> replaceText(~replacer="", "/")
  |> replaceText(~replacer="", "//")
  |> replaceText(~replacer="", "\:")
  |> replaceText(~replacer="", "\.")
  |> replaceText(~replacer="", "?")
  |> replaceText(~replacer="", "=")
  |> (text => text ++ ".png")
  |> Fpath.v;

let getTexture = (imagePath: string) => {
  /* TODO: Support url paths? */
  let cacheResult = Hashtbl.find_opt(_cache, imagePath);

  switch (cacheResult) {
  | Some(r) => r
  | None =>
    let isRemote = Uri.scheme(imagePath |> Uri.of_string) |> Option.is_some;

    let fullImagePath =
      switch (isRemote) {
      | true =>
        let fileName = Sys.getcwd() |> Fpath.v;
        let normalisedImagePath =
          Fpath.append(fileName, normaliseUrl(imagePath));
        Cohttp_lwt_unix.Client.get(Uri.of_string(imagePath))
        |> Lwt.map(((resp, body)) => {
             Console.log(("ImagePath", imagePath));
             /* Console.log(("ImageResponse", resp)); */
             /* Console.log(("ImageBody", body)); */

             body
             |> Cohttp_lwt.Body.to_string
             |> Lwt.map(body => {
                  Console.log((
                    "Filepath",
                    normalisedImagePath |> Fpath.to_string,
                  ));

                  switch (Bos.OS.File.write(normalisedImagePath, body)) {
                  | Ok(v) => Console.log(("Successfully wrote file", v))

                  | Error(`Msg(msg)) => Console.log(("Error:", msg))
                  };
                })
             |> ignore;
           })
        |> ignore;

        Console.log(("ImagePath - Stripped", normalisedImagePath));

        Environment.getAssetPath(normalisedImagePath |> Fpath.to_string);
      | _ => Environment.getAssetPath(imagePath)
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

    let imageLoadPromise = Image.load(fullImagePath);

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
