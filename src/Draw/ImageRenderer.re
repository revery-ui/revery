open Revery_Core;
open LetOperators;

module Log = (val Log.withNamespace("Revery.ImageRenderer"));

type cache = Hashtbl.t(string, option(Skia.Image.t));
let _cache: cache = Hashtbl.create(100);

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

let getTexture = (imagePath: string) => {
  let isRemote = imagePath |> Uri.of_string |> Uri.scheme |> Option.is_some;

  let imagePath = isRemote ? imagePath : Environment.getAssetPath(imagePath);

  Console.log("Image Path: " ++ imagePath);

  let cacheResult = Hashtbl.find_opt(_cache, imagePath);

  switch (cacheResult) {
  | Some(r) =>
    switch (r) {
    | Some(cachedImage) => Lwt.return(Some(cachedImage))
    | None => Lwt.return(None)
    }
  | None =>
    if (isRemote) {
      let.map result =
        Fetch.(
          {
            Console.log("Image is remote: " ++ imagePath);
            Log.info("Image is remote: " ++ imagePath);

            let.flatMapOk {Response.body, headers} = get(imagePath);

            Console.log(("Got body: ", body));

            Lwt.return(Ok((Response.Body.toString(body), headers)));
          }
        );

      switch (result) {
      | Ok((body, headers)) =>
        let fileNameCleaned =
          Fpath.(
            append(
              v(Environment.getWorkingDirectory()),
              v(Utils.removeNonAlphanumeric(imagePath)),
            )
          );

        let fileExtension = Fpath.v(Utils.getFileExtension(headers));
        let filePath =
          Fpath.add_ext(Fpath.to_string(fileExtension), fileNameCleaned);

        Log.debug("Filepath to write to: " ++ Fpath.to_string(filePath));

        Console.log("Filepath to write to: " ++ Fpath.to_string(filePath));

        let image =
          switch (Bos.OS.File.write(filePath, body)) {
          | Ok(_success) =>
            let imagePath = Fpath.to_string(filePath);

            Log.info("Loading from path: " ++ imagePath);

            let data = Skia.Data.makeFromFileName(imagePath);
            Log.info("Got data.");

            let img = Skia.Image.makeFromEncoded(data, None);
            Log.info("Got image.");

            Console.log(("Got image: ", img));

            Hashtbl.replace(_cache, Fpath.to_string(fileNameCleaned), img);

            img;
          | Error(`Msg(error)) =>
            Console.log("Error writing: " ++ error);
            Log.error("Error writing remote image to file: " ++ error);
            None;
          };

        image;
      | Error(msg) =>
        Console.log("Error: " ++ msg);
        Log.error("Remote image error: " ++ msg);
        None;
      };
    } else {
      Log.info("Loading from path: " ++ imagePath);

      let data = Skia.Data.makeFromFileName(imagePath);

      Log.info("Got data.");

      let img = Skia.Image.makeFromEncoded(data, None);

      Log.info("Got image.");

      Hashtbl.replace(_cache, imagePath, img);

      Lwt.return(img);
    }
  };
};
