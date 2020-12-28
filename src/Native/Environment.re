/* If you change these you *must* keep environment.c up to date. */
type os =
  /* Int values */
  | Unknown // 0
  | Android // 1
  | IOS // 2
  | Windows // 3
  | Browser // 4
  /* Block values */
  | Mac(int, int, int) // 0
  | Linux(int, int, int, int); // 1

external getOS: unit => os = "revery_getOperatingSystem";
