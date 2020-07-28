type t;

type title = [ | `Text(string) | `Image(string)];

/**
 * make
 *
 * Takes a [title] of either `Text(string) or `Image(string).
 * Where `Image(string) takes an absolute path to a file.
 *
 * Examples:
 *   let tray = Tray.make(~title=`Text("Some Title"), ());
 *   Tray.make(~title=`Image("/absolute/path/to/image.png"), ()) |> ignore;
 *   Tray.make(~title=`Image(Environment.getAssetPath("some_asset_image.png"), ()) |> ignore;
 */
let make: (~title: title, unit) => t;
