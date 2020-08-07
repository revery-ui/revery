%import
"config.h";

%if
USE_GTK == 1;

open {
       external c_startGtkEventLoop: (unit => bool) => unit =
         "revery_startGtkEventLoop";
     };

let renderLoop = c_startGtkEventLoop;

[%%endif];
