/*
 * Lwt_integration.re
 *
 * Lwt engine loop integration
 *
 * The Revery 'app-model' doesn't fit into the Lwt_main.run model of execution - which
 * blocks the main thread until the promise is complete.
 *
 * However, a slightly lower-level primitive, Lwt_engine, which Lwt_main calls,
 * lets us fit the event-queue based model into Revery.
 *
 * This module is responsible for facilitating that integration.
 */

/*
 * We're using some Lwt internals, so need to disable this warning...
 */
[@ocaml.warning "-3"];
open Revery;

let startEventLoop = () => {
  let yielded = Lwt_sequence.create();

  Tick.interval(
    _ =>
      Performance.bench("Lwt engine pump", () => {
        Lwt.wakeup_paused();
        Lwt_engine.iter(false);

        if (!Lwt_sequence.is_empty(yielded)) {
          let tmp = Lwt_sequence.create();
          Lwt_sequence.transfer_r(yielded, tmp);
          Lwt_sequence.iter_l(wakener => Lwt.wakeup(wakener, ()), tmp);
        };
      }),
    Seconds(0.),
  );
};
