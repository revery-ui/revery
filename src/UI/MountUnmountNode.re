open Revery_Core;
open Revery_Draw;
open Revery_Draw.SolidShader;

module Geometry = Revery_Geometry;
module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;

open Node;
open Style;

type mountCallback = unit => unit;
type unmountCallback = unit => unit;

let noop = () => ();

class mountUnmountNode (()) = {
  as _this;
  inherit (class node)() as _super;
  val _isMounted: ref(bool) = ref(false);
  val _onMountCallback: ref(mountCallback) = ref(noop);
  val _onUnmountCallback: ref(unmountCallback) = ref(noop);

  pub setOnMount = (cb: option(mountCallback)) => {
    switch (cb) {
    | Some(v) => _onMountCallback := v;
    | None => _onUnmountCallback := noop;
    };
  };

  pub setOnUnmount = (cb: option(unmountCallback)) => {
    switch (cb) {
    | Some(v) => _onUnmountCallback := v;
    | None => _onUnmountCallback := noop;
    };
  }
};
