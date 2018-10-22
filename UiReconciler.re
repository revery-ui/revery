/* Reconciler.re
 * 
 * This implements a reconciler for our UI primitives
 */

type primitives = 
    | View
    | Text
    | Image;


let createInstance = (_prim) => {
    print_endline ("create instance");
};

let appendChild = (_parent, _child) => {
    print_endline ("appendChild");
};

let removeChild = (_parent, _child) => {
    print_endline ("removeChild");
};

let updateInstance = () => {
    print_endline ("updateElement");
};

let replaceChild = () => {
    removeChild(parent, oldChild);
    appendChild(parent, newChild);
};
