let def_key = Brisk_reconciler.Key.create();
let view:
  ref(
    (~key: Brisk_reconciler.Key.t=?, ~window: Revery_Core.Window.t, ~initialExample: string, ~setGen: (int => int) => unit, unit) =>
    Brisk_reconciler.element(ViewNode.viewNode),
  ) =
  ref((~key as _=def_key, ~window as _, ~initialExample as _, ~setGen as _, ()) =>
    failwith("Uncaught ReferenceError: view is not defined\n")
  );

let gen = ref(0);
