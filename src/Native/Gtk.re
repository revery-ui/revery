%import
"config.h";

%ifdef
USE_GTK;

open {
       external c_startGtkEventLoop: (unit => bool) => unit =
         "revery_startGtkEventLoop";
     };

let renderLoop = c_startGtkEventLoop;

[%%endif];
