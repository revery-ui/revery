open Revery_Core;
open LwtLetOperators;
module Log = (val Log.withNamespace("Revery.IO.Image"));

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

  let textToAlphaNumeric = text =>
    Str.(global_replace(regexp("[^a-zA-Z0-9_]"), "", text));

  let createFilePath = (url, ~fileExtension) => {
    let fileNameCleaned =
      Fpath.(
        append(
          v(Environment.getTempDirectory()),
          v(textToAlphaNumeric(url)),
        )
      );

    let filePath = Fpath.add_ext(fileExtension, fileNameCleaned);

    Fpath.to_string(filePath);
  };
};

type urlCacheItem =
  | Image(option(Skia.Image.t))
  | Pending;

type urlCache = Hashtbl.t(string, urlCacheItem);
let urlContextCache: urlCache = Hashtbl.create(100);

let fromUrl = url => {
  let cacheResult = Hashtbl.find_opt(urlContextCache, url);

  switch (cacheResult) {
  | Some(result) =>
    switch (result) {
    | Image(maybeImage) => Lwt.return(Ok(maybeImage))
    | Pending => Lwt.return(Ok(None))
    }
  | None =>
    Hashtbl.replace(urlContextCache, url, Pending);
    open Fetch;

    let.flatMapOk {Response.body, headers, _} = get(url);

    let data = Body.toString(body);
    let mediaType =
      headers
      |> List.find_opt(((k, _v)) =>
           String.lowercase_ascii(k) == "content-type"
         )
      |> Option.map(((_k, v)) => v)
      |> Option.value(~default="");

    let fileExtension = Utility.mediaTypeToFileExtension(mediaType);
    let fileName = Utility.createFilePath(url, ~fileExtension);

    let.flatMapOk result = File.write(~path=fileName, data);
    let maybeData = Skia.Data.makeFromFileName(fileName);
    let.flatMapOk _result = File.delete(~path=fileName);

    let maybeSkiaImage =
      Option.bind(maybeData, data => Skia.Image.makeFromEncoded(data, None));

    Hashtbl.replace(urlContextCache, url, Image(maybeSkiaImage));

    Lwt.return(Ok(maybeSkiaImage));
  };
};

type cache = Hashtbl.t(string, option(Skia.Image.t));
let contextCache: cache = Hashtbl.create(100);

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

    let maybeData = Skia.Data.makeFromFileName(imagePath);

    Log.info("Got data.");

    let maybeImage =
      Option.bind(maybeData, data => Skia.Image.makeFromEncoded(data, None));

    Hashtbl.replace(contextCache, imagePath, maybeImage);

    maybeImage;
  };
};
