/*
 * Protocol
 * 
 * Types for communication: worker <-> renderer
 */

open Js_of_ocaml;

open Types;

module ToWorker {
    type t =
    | SourceCodeUpdated(Js.t(Js.js_string));
}

module ToRenderer {
    type t =
    | Ready
    | Compiling
    | SourceCodeCompiled(result(unit, unit))
    | Updates(list(Types.updates))
};
