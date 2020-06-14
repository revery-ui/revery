/*
 * Include Revery_Core in the top level 'Revery' module
 * Otherwise it's confusing when to open 'Revery' vs 'Revery.Core'
 */
include Revery_Core;

/* Courtesy of @reason-native/console - a console-like API for native! */
module Console = Console;

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

module Utility = Revery_Utility;

module SVG = Revery_SVG;

let hook_p:
  ref(
    (
      ~key: Brisk_reconciler.Key.t=?,
      ~window: Window.t,
      ~initialExample: string,
      unit
    ) =>
    Brisk_reconciler.element(Revery_UI.viewNode),
  ) =
  ref((~key as _=?, ~window as _, ~initialExample as _, ()) =>
    failwith("Uncaught ReferenceError: hook_p is not defined")
  );
