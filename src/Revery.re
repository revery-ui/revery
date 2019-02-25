
/*
 * Include Revery_Core in the top level 'Revery' module
 * Otherwise it's confusing when to open 'Revery' vs 'Revery.Core'
 */
include Revery_Core;

module Geometry = Revery_Geometry;
module Math = Revery_Math;
module Shaders = Revery_Shaders;
module UI = {
  include Revery_UI;

  /*
   * Include Components such that consumers access it via:
   * Revery.UI.Components
   */
  module Components = Revery_UI_Components;
};
module App = Core.App;
module Window = Core.Window;
module Time = Core.Time;

module Platform = {
  include Platform;
};
