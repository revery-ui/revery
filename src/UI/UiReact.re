open Brisk_core;

module React = ReactCore.Make(UiReconciler.Reconciler);
include React;
