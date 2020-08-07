type t;

/**
 * make
 *
 * Takes an optional [imagePath] which is an absolute path to a in image-file.
 * Returns a newly created Tray.t;
 *
 * Examples:
 *   Tray.make(~imagePath="/absolute/path/to/image.png", ()) |> ignore;
 *   Tray.make(~imagePath=Environment.getAssetPath("some_asset_image.png", ()) |> ignore;
 *   let tray = Tray.make();
 */
let make: (~imagePath: string=?, unit) => t;
