/*
 * GarbageCollector.re
 *
 * Thin wrapper over the 'Gc' module to support some
 * basic primitives for tuning performance in the
 * Revery app model.
 */

/* Increase minor heap size: 256kb -> 8MB */
/*
 * Some more info on why this is helpful:
 * https://md.ekstrandom.net/blog/2010/06/ocaml-memory-tuning
 */
let minorHeapSize = 8 * 1024 * 1024;

/*
 * Amortize major collections across frames
 */
let defaultSliceSize = minorHeapSize / 60;

let tune = () =>
  /* Gc tuning is only applicable in the native space */
  if (Environment.isNative) {
    let settings = Gc.get();

    Gc.set({...settings, minor_heap_size: minorHeapSize});
  };

let minor = () =>
  if (Environment.isNative) {
    let _ = Gc.major_slice(defaultSliceSize);
    ();
  };

let full = () =>
  if (Environment.isNative) {
    Gc.full_major();
  };

let counters = () =>
  if (Environment.isNative) {
    Gc.counters();
  } else {
    (0., 0., 0.);
  };
