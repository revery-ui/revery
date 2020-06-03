type t =
  | Undefined
  | Thin
  | UltraLight
  | Light
  | Normal
  | Medium
  | SemiBold
  | Bold
  | UltraBold
  | Heavy;

let toInt = (v: t) => {
  switch (v) {
  // NOTE: Must be in sync with FontDescriptor.h!
  | Undefined => 0
  | Thin => 100
  | UltraLight => 200
  | Light => 300
  | Normal => 400
  | Medium => 500
  | SemiBold => 600
  | Bold => 700
  | UltraBold => 800
  | Heavy => 900
  };
};

let ofInt = (v: int) => {
  switch (v) {
  | 100 => Thin
  | 200 => UltraLight
  | 300 => Light
  | 400 => Normal
  | 500 => Medium
  | 600 => SemiBold
  | 700 => Bold
  | 800 => UltraBold
  | 900 => Heavy
  | _ => Undefined
  };
};

let show = (v: t) => {
  // NOTE: Must be in sync with FontDescriptor.h!
  switch (v) {
  | Undefined => "Undefined"
  | Thin => "Thin"
  | UltraLight => "UltraLight"
  | Light => "Light"
  | Normal => "Normal"
  | Medium => "Medium"
  | SemiBold => "SemiBold"
  | Bold => "Bold"
  | UltraBold => "UltraBold"
  | Heavy => "Heavy"
  };
};
