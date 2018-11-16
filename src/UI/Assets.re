/**
 * Assets
 * 
 * Lazily initialized assets to be re-used across frames
 */

module Geometry = Revery_Geometry;

let solidShader = Lazy.force(SolidShader.create);
let fontShader = Lazy.force(FontShader.create);
let textureShader = Lazy.force(TextureShader.create));

let quad = Lazy.force(Geometry.Quad.create);
