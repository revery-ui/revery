open Skia;

let makeSurface = (width, height) => {
  let imageInfo = Skia.ImageInfo.make(width, height, Rgba8888, Premul, None);
  Surface.makeRaster(imageInfo, 0, None);
};
