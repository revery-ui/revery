/**
 * fromUrl
 *
 * Given a network file-path this returns a promise, holding an option(Skia.Image.t)
 *
 * Examples:
 * let maybeImage: Lwt.t(option(Skia.Image.t)) = Image.fromUrl("https://example.com/hello.png");
 */
// let fromUrl: string => Lwt.t(option(Skia.Image.t));

/**
 * fromAssetPath
 *
 * Given a path, reads an image-file from disk, returning an option-type.
 *
 * Examples:
 * let maybeImage: option(Skia.Image.t) = Image.fromAssetPath("example.png");
 */
let fromAssetPath: string => option(Skia.Image.t);
