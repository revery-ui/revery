/* Event.re */

type cb('a) = 'a => unit;

type t('a) = ref(list(cb('a)));

let create = () => ref([]);

let subscribe = (evt: t('a), f: cb('a)) => {
  evt := List.append(evt^, [f]);
  let unsubscribe = () => {
    evt := List.filter(f => f !== f, evt^);
  };
  unsubscribe;
};

let dispatch = (evt: t('a), v: 'a) => List.iter(c => c(v), evt^);
