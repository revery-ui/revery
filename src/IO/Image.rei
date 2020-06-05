/**
 * fromUrl
 *
 * Given a network file-path this returns a promise,
 * holding either a Ok(option(Skia.Image.t)) or an Error(string).
 *
 * Examples:
 * let result = Image.fromUrl("https://example.com/hello.png");
 */
let fromUrl: string => Lwt.t(result(option(Skia.Image.t), string));

/**
 * fromAssetPath
 *
 * Given a path, reads an image-file from disk, returning an option-type.
 *
 * Examples:
 * let result = Image.fromAssetPath("example.png");
 */
let fromAssetPath: string => option(Skia.Image.t);
