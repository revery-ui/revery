print_endline ("hello");

module UiReact = Reactify.make(UiReconciler);

let view = (~children, ~style=Style.defaultStyle, ()) => 
    UiReact.primitiveComponent(View, ~children);

