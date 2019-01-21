open Reglfw.Glfw;

/* This module must use lazy values for the cursors, because GLFW will return
   [GLFW_NOT_INITIALIZED] (which renders as an arrow) if this gets called before
   [glfwInit]. Since these are global variables, they WILL be initialized first.
   We could also return a new cursor every time, but this would cause a memory
   leak. */
type t = [ | `Arrow | `Text | `Pointer | `Crosshair | `HResize | `VResize];

let arrow_lazy = lazy (glfwCreateStandardCursor(GLFW_ARROW_CURSOR));
let text_lazy = lazy (glfwCreateStandardCursor(GLFW_IBEAM_CURSOR));
let pointer_lazy = lazy (glfwCreateStandardCursor(GLFW_HAND_CURSOR));
let crosshair_lazy = lazy (glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR));
let horizontalResize_lazy =
  lazy (glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR));
let verticalResize_lazy =
  lazy (glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR));

let toGlfwCursor = cursorType => {
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

let arrow = `Arrow;
let text = `Text;
let pointer = `Pointer;
let crosshair = `Crosshair;
let horizontalResize = `HResize;
let verticalResize = `VResize;
