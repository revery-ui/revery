open Reglfw;

type t = Glfw.Monitor.t;

type size = {
  width: int,
  height: int,
};

let getPrimaryMonitor = () => Glfw.glfwGetPrimaryMonitor();

let calculateDPI = (modeDimension, physicalDimension) =>
  float_of_int(modeDimension) /. (float_of_int(physicalDimension) /. 25.4);

let getScaleFactor = () => {
  let monitor = Glfw.glfwGetPrimaryMonitor();
  let vidMode = Glfw.glfwGetVideoMode(monitor);
  let physicalSize = Glfw.glfwGetMonitorPhysicalSize(monitor);
  let dpiH = calculateDPI(vidMode.height, physicalSize.height);
  let dpiW = calculateDPI(vidMode.width, physicalSize.width);
  let avgDPI = dpiW +. dpiH /. 2.0;
  let scaleFactor = int_of_float(avgDPI) / 96;
  scaleFactor >= 1 ? scaleFactor : 1;
};

let getSize = monitor => {
  let monitorSize = Glfw.glfwGetVideoMode(monitor);
  let ret: size = {width: monitorSize.width, height: monitorSize.height};
  ret;
};
