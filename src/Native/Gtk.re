%import
"config.h";

%ifdef
USE_GTK;

open {
       external c_gtkEventsPending: unit => bool = "revery_gtkEventsPending";
       external c_gtkMainIteration: unit => bool = "revery_gtkMainIteration";
     };

let eventsPending = c_gtkEventsPending;
let mainIteration = c_gtkMainIteration;

[%%endif];
