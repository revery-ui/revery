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

module Option = {
  let value = (~default, a) =>
    switch (a) {
    | Some(v) => v
    | None => default
    };

  let map = (f, value) =>
    switch (value) {
    | Some(v) => Some(f(v))
    | None => None
    };

  let isSome =
    fun
    | Some(_) => true
    | _ => false;
};

module Utils = {
  module Text = {
    let onlyAlphaNumeric = text =>
      Str.(global_replace(regexp("[^a-zA-Z0-9_]"), "", text));
  };

  module RemoteImage = {
    let extensionOfMediaType =
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

    type t = {
      data: string,
      fileExtension: string,
    };

    let fetch = url =>
      Fetch.(
        get(url)
        |> Lwt.map(
             fun
             | Ok({Response.body, headers, _}) => {
                 let fileExtension =
                   headers
                   |> List.find_opt(((k, _v)) => k == "content-type")
                   |> Option.map(((_k, v)) => v)
                   |> Option.value(~default="")
                   |> extensionOfMediaType;

                 Some({data: Response.Body.toString(body), fileExtension});
               }
             | Error(_) => None,
           )
      );
  };
};

let getTexture = (imagePath: string) => {
  let cacheResult = Hashtbl.find_opt(_cache, imagePath);
  let remoteImagePath = ref(None);

  switch (cacheResult) {
  | Some(cachedResult) => cachedResult
  | None =>
    /* will this suffice? */
    let isRemote = imagePath |> Uri.of_string |> Uri.scheme |> Option.isSome;

    let imageLoadPromise =
      if (isRemote) {
        let%lwt remoteImage = Utils.RemoteImage.fetch(imagePath);

        let image =
          switch (remoteImage) {
          | Some(remoteImage) =>
            let filePath =
              Fpath.append(
                Environment.getTempDirectory() |> Fpath.v,
                Utils.Text.onlyAlphaNumeric(imagePath)
                ++ remoteImage.fileExtension
                |> Fpath.v,
              );

            switch (Bos.OS.File.write(filePath, remoteImage.data)) {
            | Ok(_unit) =>
              remoteImagePath := Some(filePath |> Fpath.to_string);
              Image.load(filePath |> Fpath.to_string);
            | Error(`Msg(error)) =>
              /* TODO: handle this */
              Log.error("Error creating temporary image:", error);
              failwith("Could not write image to file");
            };

          | None => failwith("Error while fetching image")
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

      switch (remoteImagePath^) {
      | Some(remoteImage) =>
        switch (Bos.OS.File.delete(~must_exist=true, remoteImage |> Fpath.v)) {
        | Ok(_) => ()
        | Error(`Msg(error)) =>
          /* TODO: handle error? */
          Log.error("Error deleting temporary image", error)
        }
      | None => ()
      };

      Lwt.return();
    };

    let _ = Lwt.bind(imageLoadPromise, success);
    Hashtbl.replace(_cache, imagePath, ret);
    ret;
  };
};
