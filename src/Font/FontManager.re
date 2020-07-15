/* FontManager.re
   This module is so that multiple other modules can use the same font
   manager without creating circular imports */

let instance = Skia.FontManager.makeDefault();
