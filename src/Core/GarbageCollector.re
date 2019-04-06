/*
 * GarbageCollector.re
 *
 * Thin wrapper over the 'Gc' module to support some
 * basic primitives for tuning performance in the
 * Revery app model.
 */

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
