let def_key = Brisk_reconciler.Key.create();
let view:
  ref(
    (~key: Brisk_reconciler.Key.t=?, ~win: Revery_Core.Window.t, ~setGen: (int => int) => unit, unit) =>
    Brisk_reconciler.element(React.reveryNode),
  ) =
  ref((~key as _=def_key, ~win as _, ~setGen as _, ()) =>
    failwith("Uncaught ReferenceError: view is not defined\n")
  );

let gen = ref(0);
