open Brisk_core;

module Reconciler = UiReconciler.Reconciler;

let onStale = Reconciler.onStale;

module React = ReactCore.Make(Reconciler);
include React;
