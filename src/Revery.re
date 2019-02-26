/*
 * Include Revery_Core in the top level 'Revery' module
 * Otherwise it's confusing when to open 'Revery' vs 'Revery.Core'
 */
include Revery_Core;

/* Courtesy of @reason-native/console - a console-like API for native! */
module Console = Console;

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

module Platform = {
  include Platform;
};
