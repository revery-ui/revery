open Revery_Core;
module Log = (val Log.withNamespace("Revery.IO.File"));

let delete = (~path as filePath) => {
  switch (Fpath.of_string(filePath)) {
  | Ok(fpath) =>
    switch (Bos.OS.File.delete(~must_exist=true, fpath)) {
    | Ok(_) =>
      Log.info("Deleted temporary image.");
      Lwt.return(Ok());
    | Error(`Msg(error)) =>
      Log.error("Error deleting temporary image" ++ error);
      Lwt.return(Error(error));
    }
  | Error(`Msg(error)) => Lwt.return(Error(error))
  };
};

let write = (~path, data) => {
  switch (Fpath.of_string(path)) {
  | Ok(fpath) =>
    switch (Bos.OS.File.write(fpath, data)) {
    | Ok(_success) =>
      Log.debug("Successfully wrote data to file: " ++ path);

      Lwt.return(Ok());
    | Error(`Msg(error)) =>
      Log.error("Error writing data to file: " ++ error);
      Lwt.return(Error(error));
    }
  | Error(`Msg(error)) => Lwt.return(Error(error))
  };
};
