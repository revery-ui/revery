open Revery_Core;

module Log = (val Log.withNamespace("Revery.HotReload"));

let persistState =
    (
      ~serialize: 'state => 'serializedState,
      ~deserialize: 'serializedState => 'state,
      initialState: 'state,
    ) => {
  let onUpdate = state =>
    switch (Sys.getenv_opt("REVERY_HOTRELOAD_FILE")) {
    | Some(path) =>
      let outChannel = open_out(path);
      let serializedState = state |> serialize;
      Marshal.to_channel(outChannel, serializedState, [Closures]);
    | None => Log.warn("Environment variable not set.")
    };

  let state =
    switch (Sys.getenv_opt("REVERY_HOTRELOAD_FILE")) {
    | Some(path) =>
      if (Sys.file_exists(path)) {
        let inChannel = open_in(path);
        let serializedState = Obj.magic(Marshal.from_channel(inChannel));
        serializedState |> deserialize;
      } else {
        initialState;
      }
    | None => initialState
    };

  (state, onUpdate);
};
