open Revery_Core;
//open LwtLetOperators;

module Log = (val Log.withNamespace("Revery.IO.Image"));

module Internal = {
  //  let mediaTypeToFileExtension =
  //    fun
  //    | "image/apng" => ".apng"
  //    | "image/bmp" => ".bmp"
  //    | "image/gif" => ".gif"
  //    | "image/x-icon" => ".ico"
  //    | "image/jpeg" => ".jpg"
  //    | "image/jpg" => ".jpg"
  //    | "image/png" => ".png"
  //    | "image/svg+xml" => ".svg"
  //    | "image/tiff" => ".tif"
  //    | "image/webp" => ".webp"
  //    | _ => ".jpg";
  //  let textToAlphaNumeric = text =>
  //    Str.(global_replace(regexp("[^a-zA-Z0-9_]"), "", text));
  //  let createFilePath = (url, ~fileExtension) => {
  //    let fileNameCleaned =
  //      Fpath.(
  //        append(
  //          v(Environment.getTempDirectory()),
  //          v(textToAlphaNumeric(url)),
  //        )
  //      );
  //
  //    let filePath = Fpath.add_ext(fileExtension, fileNameCleaned);
  //
  //    Fpath.to_string(filePath);
  //  };
};

// NOTE: The reason for these different states is to hold up any
// additional requests until we've actually got a response
// from the first one
type urlCacheItem =
  | Image(option(Skia.Image.t))
  | Pending;

type urlCache = Hashtbl.t(string, urlCacheItem);
//let urlContextCache: urlCache = Hashtbl.create(100);

// NOTE: These could be moved elsewhere
//let fromUrl = url => {
//  let cacheResult = Hashtbl.find_opt(urlContextCache, url);
//
//  let.flatMap result =
//    switch (cacheResult) {
//    | Some(result) =>
//      switch (result) {
//      | Image(maybeImage) => Lwt.return(Ok(maybeImage))
//      | Pending => Lwt.return(Ok(None))
//      }
//    | None =>
//      Hashtbl.replace(urlContextCache, url, Pending);
//      open Fetch;
//
//      let.flatMapOk {Response.body, headers, _} = get(url);
//
//      let data = Body.toString(body);
//      let mediaType =
//        headers
//        |> List.find_opt(((k, _v)) =>
//             String.lowercase_ascii(k) == "content-type"
//           )
//        |> Option.map(((_k, v)) => v)
//        |> Option.value(~default="");
//
//      let fileExtension = Internal.mediaTypeToFileExtension(mediaType);
//      let fileName = Internal.createFilePath(url, ~fileExtension);
//
//      let.flatMapOk _writeOk = File.write(~path=fileName, data);
//      let maybeData = Skia.Data.makeFromFileName(fileName);
//      let.flatMapOk _deleted = File.delete(fileName);
//
//      let maybeSkiaImage =
//        Option.bind(maybeData, data =>
//          Skia.Image.makeFromEncoded(data, None)
//        );
//
//      Hashtbl.replace(urlContextCache, url, Image(maybeSkiaImage));
//
//      Lwt.return(Ok(maybeSkiaImage));
//    };
//
//  Lwt.return(Result.value(result, ~default=None));
//};

type cache = Hashtbl.t(string, option(Skia.Image.t));
let contextCache: cache = Hashtbl.create(100);

let fromAssetPath = (imagePath: string) => {
  let imagePath = Environment.getAssetPath(imagePath);

  let cacheResult = Hashtbl.find_opt(contextCache, imagePath);

  switch (cacheResult) {
  | Some(maybeCachedImage) => maybeCachedImage
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
