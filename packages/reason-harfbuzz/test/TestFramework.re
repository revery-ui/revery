include Rely.Make({
  let config =
    Rely.TestFrameworkConfig.initialize({
      snapshotDir: "./__snapshots__",
      projectDir: "",
    });
});

let font =
  Harfbuzz.hb_face_from_path("./examples/Roboto-Regular.ttf") |> Result.get_ok;
