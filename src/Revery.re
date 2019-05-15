/*
 * Include Revery_Core in the top level 'Revery' module
 * Otherwise it's confusing when to open 'Revery' vs 'Revery.Core'
 */
include Revery_Core;

/* Courtesy of @reason-native/console - a console-like API for native! */
module Console = Console;

module Draw = Revery_Draw;
module Geometry = Revery_Geometry;
module Math = Revery_Math;
module Shaders = Revery_Shaders;
module UI = {
  include Revery_UI;
  include Revery_UI_Primitives;

  /*
   * Include Components such that consumers access it via:
   * Revery.UI.Components
   */
  module Components = Revery_UI_Components;
  module Hooks = Revery_UI_Hooks;
};

module Platform = {
  include Platform;
};
