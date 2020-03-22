open Revery_Core;
open LetOperators;

module Log = (val Log.withNamespace("Revery.ImageRenderer"));

type cache = Hashtbl.t(string, option(Skia.Image.t));
let contextCache: cache = Hashtbl.create(100);

module Utils = {
  let removeNonAlphanumeric = text =>
    Str.(global_replace(regexp("[^a-zA-Z0-9_]"), "", text));

  let fileExtensionOfMediaType =
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

  let getFileExtension = (headers: list(Fetch.Headers.t)) => {
    headers
    |> List.find_opt(((k, _v)) =>
         String.lowercase_ascii(k) == "content-type"
       )
    |> Option.map(((_k, v)) => v)
    |> Option.value(~default="")
    |> fileExtensionOfMediaType;
  };
};

let getTextureRemote = (imagePath: string) => {
  let cacheResult = Hashtbl.find_opt(contextCache, imagePath);

  switch (cacheResult) {
  | Some(r) =>
    switch (r) {
    | Some(cachedImage) => Lwt.return(Some(cachedImage))
    | None => Lwt.return(None)
    }
  | None =>
    let.map result =
      Fetch.(
        {
          Log.info("Image is remote: " ++ imagePath);

          let.flatMapOk {Response.body, headers} = get(imagePath);

          Lwt.return(Ok((Response.Body.toString(body), headers)));
        }
      );

    switch (result) {
    | Ok((body, headers)) =>
      let fileNameCleaned =
        Fpath.(
          append(
            v(Environment.getTempDirectory()),
            v(Utils.removeNonAlphanumeric(imagePath)),
          )
        );

      let fileExtension = Fpath.v(Utils.getFileExtension(headers));
      let filePath =
        Fpath.add_ext(Fpath.to_string(fileExtension), fileNameCleaned);

      Log.debug("Filepath to write to: " ++ Fpath.to_string(filePath));

      let image =
        switch (Bos.OS.File.write(filePath, body)) {
        | Ok(_success) =>
          let storedImagePath = Fpath.to_string(filePath);

          Log.info("Loading from path: " ++ storedImagePath);

          let data = Skia.Data.makeFromFileName(storedImagePath);
          Log.info("Got data.");

          let img = Skia.Image.makeFromEncoded(data, None);
          Log.info("Got image.");

          Hashtbl.replace(contextCache, imagePath, img);

          img;
        | Error(`Msg(error)) =>
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

let getTexture = (imagePath: string) => {
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
