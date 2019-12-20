open Sdl2;

/* This module must use lazy values for the cursors, because GLFW will return
   [GLFW_NOT_INITIALIZED] (which renders as an arrow) if this gets called before
   [glfwInit]. Since these are global variables, they WILL be initialized first.
   We could also return a new cursor every time, but this would cause a memory
   leak. */
type t = [ | `Arrow | `Text | `Pointer | `Crosshair | `HResize | `VResize];

let arrow_lazy = lazy(Cursor.createSystem(Cursor.Arrow));
let text_lazy = lazy(Cursor.createSystem(Cursor.IBeam));
let pointer_lazy = lazy(Cursor.createSystem(Cursor.Hand));
let crosshair_lazy = lazy(Cursor.createSystem(Cursor.Crosshair));
let horizontalResize_lazy = lazy(Cursor.createSystem(Cursor.SizeWE));
let verticalResize_lazy = lazy(Cursor.createSystem(Cursor.SizeNS));

let _toSdlCursor = cursorType => {
  (
    switch (cursorType) {
    | `Arrow => arrow_lazy
    | `Text => text_lazy
    | `Pointer => pointer_lazy
    | `Crosshair => crosshair_lazy
    | `HResize => horizontalResize_lazy
    | `VResize => verticalResize_lazy
    }
  )
  |> Lazy.force;
};

let setCursor = v => {
  _toSdlCursor(v) |> Cursor.set;
};

let arrow = `Arrow;
let text = `Text;
let pointer = `Pointer;
let crosshair = `Crosshair;
let horizontalResize = `HResize;
let verticalResize = `VResize;

let toString = (v: t) =>
  switch (v) {
  | `Arrow => "arrow"
  | `Text => "text"
  | `Pointer => "pointer"
  | `Crosshair => "crosshair"
  | `HResize => "hresize"
  | `VResize => "vresize"
  };
