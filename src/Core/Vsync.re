type t =
  | Immediate /* no vsync */
  | Synchronized; /* waiting for vsync */
// TODO: Enable adaptive vsync
//| Adaptive /* like vsync, but swaps buffers immediately if vertical retrace missed. Not supported everywhere */

let toInt = v =>
  switch (v) {
  | Immediate => 0
  | Synchronized => 1
  //| Adaptive => -1
  };

let toString = v =>
  switch (v) {
  | Immediate => "Immediate"
  | Synchronized => "Synchronized"
  //| Adaptive => "Adaptive"
  };
