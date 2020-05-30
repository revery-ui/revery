open NodeEvents;

let loseFocus: unit => unit;

let isFocused: Node.node => bool;

let focus: Node.node => unit;

let dispatch: event => unit;
