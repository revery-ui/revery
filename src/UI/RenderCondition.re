type t =
  | Condition(('a, 'a) => bool, 'a): t;

let initial = Condition((a, b) => a ==b, 1);

let shouldRender = (previousCondition: t, currentCondition: t) => {
  switch (previousCondition, currentCondition) {
  // If one of the conditions was always... then render.
  | (Condition(_, previousValue), Condition(predicate, newValue)) =>
    //   predicate(previousValue, newValue)
    false
  };
};
