module Reconciler = UiReconciler.Reconciler;

let onStale = Reconciler.onStale;

module React = Brisk_reconciler.Make(Reconciler);
include React;
