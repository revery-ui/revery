open Revery_Core;
open LetOperators;

module Log = (val Log.withNamespace("Revery.ImageRenderer"));

type cache = Hashtbl.t(string, option(Skia.Image.t));
let contextCache: cache = Hashtbl.create(100);

type urlCacheItem =
  | Image(option(Skia.Image.t))
  | Pending;

type urlCache = Hashtbl.t(string, urlCacheItem);
let urlContextCache: urlCache = Hashtbl.create(100);

module RemoteImage = {
  type t = {
    data: string,
    mediaType: string,
  };

  let download = url => {
    Fetch.(
      {
        Log.info("Fetching image: " ++ url);

        let.flatMapOk {Response.body, headers} = get(url);

        let data = Body.toString(body);
        let mediaType =
          headers
          |> List.find_opt(((k, _v)) =>
               String.lowercase_ascii(k) == "content-type"
             )
          |> Option.map(((_k, v)) => v)
          |> Option.value(~default="");

        Lwt.return(Ok({data, mediaType}));
      }
    );
  };

  module Utility = {
    let mediaTypeToFileExtension =
      fun
      | "image/apng" => ".apng"
      | "image/bmp" => ".bmp"
      | "image/gif" => ".gif"
      | "image/x-icon" => ".ico"
      | "image/jpeg" => ".jpg"
      | "image/jpg" => ".jpg"
      | "image/png" => ".png"
      | "image/svg+xml" => ".svg"
      | "image/tiff" => ".tif"
      | "image/webp" => ".webp"
      | _ => ".jpg";

    module Text = {
      let toAlphaNumeric = text =>
        Str.(global_replace(regexp("[^a-zA-Z0-9_]"), "", text));
    };

    let createFilePath = (url, ~fileExtension) => {
      let fileNameCleaned =
        Fpath.(
          append(
            v(Environment.getTempDirectory()),
            v(Text.toAlphaNumeric(url)),
          )
        );

      let filePath = Fpath.add_ext(fileExtension, fileNameCleaned);

      filePath;
    };
  };
};

let fromUrl = (url: string) => {
  let cacheResult = Hashtbl.find_opt(urlContextCache, url);

  switch (cacheResult) {
  | Some(result) =>
    switch (result) {
    | Image(maybeImage) =>
      switch (maybeImage) {
      | Some(cachedImage) => Lwt.return(Some(cachedImage))
      | None => Lwt.return(None)
      }
    | Pending => Lwt.return(None)
    }
  | None =>
    Hashtbl.replace(urlContextCache, url, Pending);

    let.map imageResult = RemoteImage.download(url);

    switch (imageResult) {
    | Ok({data, mediaType}) =>
      let fileExtension =
        RemoteImage.Utility.mediaTypeToFileExtension(mediaType);
      let filePath = RemoteImage.Utility.createFilePath(url, ~fileExtension);

      Log.debug("Filepath to write to: " ++ Fpath.to_string(filePath));

      let image =
        switch (Bos.OS.File.write(filePath, data)) {
        | Ok(_success) =>
          let storedImagePath = Fpath.to_string(filePath);

          Log.info("Loading from path: " ++ storedImagePath);

          let data = Skia.Data.makeFromFileName(storedImagePath);
          Log.info("Got data.");

          let skiaImage = Skia.Image.makeFromEncoded(data, None);
          Log.info("Got image.");

          Hashtbl.replace(urlContextCache, url, Image(skiaImage));

          /* Until we offer options for caching, lets delete the image-file */
          switch (Bos.OS.File.delete(~must_exist=true, filePath)) {
          | Ok(_) => Log.info("Deleted temporary image.")
          | Error(`Msg(error)) =>
            Log.error("Error deleting temporary image" ++ error)
          };

          skiaImage;
        | Error(`Msg(error)) =>
          Hashtbl.remove(urlContextCache, url);
          Log.error("Error writing remote image to file: " ++ error);
          None;
        };

      image;
    | Error(msg) =>
      Log.error("Remote image error: " ++ msg);
      None;
    };
  };
};

let fromAssetPath = (imagePath: string) => {
  let imagePath = Environment.getAssetPath(imagePath);

  let cacheResult = Hashtbl.find_opt(contextCache, imagePath);

  switch (cacheResult) {
  | Some(r) =>
    switch (r) {
    | Some(cachedImage) => Some(cachedImage)
    | None => None
    }
  | None =>
    Log.info("Loading from path: " ++ imagePath);

    let data = Skia.Data.makeFromFileName(imagePath);

    Log.info("Got data.");

    let img = Skia.Image.makeFromEncoded(data, None);

    Log.info("Got image.");

    Hashtbl.replace(contextCache, imagePath, img);

    img;
  };
};
