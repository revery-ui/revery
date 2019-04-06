/*
 * Protocol
 *
 * Types for communication: worker <-> renderer
 */

open Js_of_ocaml;

open Types;

module ToWorker = {
  type nodeMeasurement = {
    id: int,
    dimensions: Revery.UI.Dimensions.t,
  };

  type t =
    | SourceCodeUpdated(Js.t(Js.js_string))
    | Measurements(list(nodeMeasurement))
    | KeyboardEvent(Revery_Core.Events.internalKeyboardEvent)
    | MouseEvent(Revery_Core.Events.internalMouseEvents);
};

module ToRenderer = {
  type t =
    | Ready
    | Compiling
    | Output(Js.t(Js.Unsafe.any))
    | SourceCodeCompiled(result(unit, unit))
    | Updates(list(Types.updates));
};
