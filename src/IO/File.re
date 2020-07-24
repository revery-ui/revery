open Revery_Core;
module Log = (val Log.withNamespace("Revery.IO.File"));

let delete = filePath => {
  switch (Fpath.of_string(filePath)) {
  | Ok(fpath) =>
    switch (Bos.OS.File.delete(~must_exist=true, fpath)) {
    | Ok(_) =>
      Log.info("Deleted file: " ++ filePath);
      Lwt.return(Ok());
    | Error(`Msg(error)) =>
      Log.error("Error deleting file: " ++ error);
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

let read = path => {
  switch (Fpath.of_string(path)) {
  | Ok(fpath) =>
    switch (Bos.OS.File.read(fpath)) {
    | Ok(data) =>
      Log.debug("Successfully read file: " ++ path);
      Lwt.return(Ok(data));
    | Error(`Msg(error)) =>
      Log.error("Error reading file: " ++ error);
      Lwt.return(Error(error));
    }
  | Error(`Msg(error)) => Lwt.return(Error(error))
  };
};
