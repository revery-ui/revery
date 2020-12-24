/* If you change these you *must* keep environment.c up to date. */
type os =
  /* Int values */
  | Unknown // 0
  | Android // 1
  | IOS // 2
  | Linux // 3
  | Windows // 4
  | Browser // 5
  /* Block values */
  | Mac(int, int, int); // 0

external getOS: unit => os = "revery_getOperatingSystem";
