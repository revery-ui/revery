open Reglfw;

type t = unit;

type size = {
  width: int,
  height: int,
};

let getPrimaryMonitor = () => ();

let calculateDPI = (modeDimension, physicalDimension) =>
  float_of_int(modeDimension) /. (float_of_int(physicalDimension) /. 25.4);

let getScaleFactor = () => {
  /*let monitor = Glfw.glfwGetPrimaryMonitor();
  let vidMode = Glfw.glfwGetVideoMode(monitor);
  let physicalSize = Glfw.glfwGetMonitorPhysicalSize(monitor);
  let dpiH = calculateDPI(vidMode.height, physicalSize.height);
  let dpiW = calculateDPI(vidMode.width, physicalSize.width);
  let avgDPI = (dpiW +. dpiH) /. 2.0;
  let rawScaleFactor = avgDPI /. 96.;
  /**
     NOTE: int_of_float aggressively shaves off any fractional values
     e.g. int_of_float 2.93 results in 2, this means the dpi we calculate
     is rounded down
   */
  let scaleFactor = int_of_float(rawScaleFactor);
  scaleFactor >= 1 ? scaleFactor : 1;*/
  1.0
};

let getSize = monitor => {
  /*let monitorSize = Glfw.glfwGetVideoMode(monitor);
  let ret: size = {width: monitorSize.width, height: monitorSize.height};
  ret;*/
  { width: 0, height: 0};
};
