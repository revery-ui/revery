open NodeEvents;

type active = {
  handler: event => unit,
  id: int,
};
type focused = ref(option(active));

let getFocused: unit => option(active);

let loseFocus: unit => unit;

let isFocused: Node.node => bool;

let focus: Node.node => unit;
