/**
 * Assets
 * 
 * Lazily initialized assets to be re-used across frames
 */

module Lazy = Revery_Core.Lazy;
module Geometry = Revery_Geometry;

let solidShader = Lazy.make(() => SolidShader.create());
let fontShader = Lazy.make(() => FontShader.create());
let textureShader = Lazy.make(() => TextureShader.create());

let quad = Lazy.make(() => Geometry.Quad.create());
