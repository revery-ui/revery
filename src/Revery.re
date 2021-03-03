/*
 * Include Revery_Core in the top level 'Revery' module
 * Otherwise it's confusing when to open 'Revery' vs 'Revery.Core'
 */
include Revery_Core;

module Font = Revery_Font;
module Draw = Revery_Draw;
module Math = Revery_Math;
module Native = Revery_Native;

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

module Debug = {
  let enable = Revery_Draw.DebugDraw.enable;
  let disable = Revery_Draw.DebugDraw.disable;
  let isEnabled = Revery_Draw.DebugDraw.isEnabled;
};

module Utility = {
  include Revery_Utility;
};

module SVG = Revery_SVG;
