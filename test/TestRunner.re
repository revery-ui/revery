// On Windows, because this is linked against the '-mwindows' flag
// (for a GUI app), we need to manually allocate a console.
if (Sys.win32) {
  let _: int = Sdl2.Platform.win32AttachConsole();
  // Unfortunately, the colors aren't showing up correctly with the
  // attached console - so I'll disable them for now.
  Pastel.setMode(Pastel.Disabled);
};

Revery_Core_Test.TestFramework.cli();
Revery_Math_Test.TestFramework.cli();
Revery_UI_Test.TestFramework.cli();

Skia_Test.TestFramework.cli();
