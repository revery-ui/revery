open Revery_Draw;

type size = {
  width: int,
  height: int,
};

type font = {
  family: string,
  size: float,
  xHeight: float, // '1 ex', x-height of the elementâs font
  chWidth: float // '1 ch', width of the "0" (ZERO, U+0030) glyph in the elementâs font
};

type context = {
  defs: list(Model.definition),
  font,
  viewport: Model.viewport,
  container: size,
  rootFontSize: float, // '1 rem', font size of the root element
  canvas: CanvasContext.t,
};
