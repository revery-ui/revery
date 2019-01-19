/*
 * GarbageCollector.re
 *
 * Thin wrapper over the 'Gc' module to support some
 * basic primitives for tuning performance in the
 * Revery app model.
 */

let tune = () =>
  /* Gc tuning is only applicable in the native space */
  if (Environment.isNative) {
    let settings = Gc.get();

    /* Increase minor heap size: 256kb -> 8MB */
    /*
     * Some more info on why this is helpful:
     * https://md.ekstrandom.net/blog/2010/06/ocaml-memory-tuning
     */
    let minorHeapSize = 8 * 1024 * 1024;
    Gc.set({...settings, minor_heap_size: minorHeapSize});
  };

let minor = () =>
  if (Environment.isNative) {
    Gc.minor();
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
