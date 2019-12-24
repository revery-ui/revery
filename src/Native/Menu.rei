let menuSupported: unit => bool;

type menu;

let createMenu: unit => menu;

let make:
  (~key: Brisk_reconciler.Key.t=?, unit) => Brisk_reconciler.element(menu);
