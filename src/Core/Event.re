/* Event.re */

module Fanout = {
  /* An event where subscribers can respond with a value */
  type cb('a, 'b) = 'a => 'b;

  type t('a, 'b) = ref(list(cb('a, 'b)));

  let dispatch = (evt: t('a, 'b), merge: ('b, 'b) => 'b, zero: 'b, v: 'a) => {
    List.fold_left((acc, listener) => merge(acc, listener(v)), zero, evt^);
  };

  let subscribe = (evt: t('a, 'b), f: cb('a, 'b)) => {
    evt := List.append(evt^, [f]);
    let unsubscribe = () => {
      evt := List.filter(f => f !== f, evt^);
    };
    unsubscribe;
  };
};

/* an event is just a fanout with a `unit` return type, but we duplicate the definition for simpler type errors */

type cb('a) = 'a => unit;

type t('a) = ref(list(cb('a)));

let create = () => ref([]);

let subscribe = (evt: t('a), f: cb('a)) => {
  Fanout.subscribe(evt, f);
};

let dispatch = (evt: t('a), v: 'a) => List.iter(c => c(v), evt^);
