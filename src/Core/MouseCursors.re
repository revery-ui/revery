open Reglfw.Glfw;

/* This module must use lazy values for the cursors, because GLFW will return
   [GLFW_NOT_INITIALIZED] (which renders as an arrow) if this gets called before
   [glfwInit]. Since these are global variables, they WILL be initialized first.
   We could also return a new cursor every time, but this would cause a memory
   leak. */
type t = Lazy.t(glfwCursor);

let toGlfwCursor = (c) => Lazy.force(c);

let arrow = lazy(glfwCreateStandardCursor(GLFW_ARROW_CURSOR));
let text = lazy(glfwCreateStandardCursor(GLFW_IBEAM_CURSOR));
let pointer = lazy(glfwCreateStandardCursor(GLFW_HAND_CURSOR));
let crosshair = lazy(glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR));
let horizontalResize = lazy(glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR));
let verticalResize = lazy(glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR));
