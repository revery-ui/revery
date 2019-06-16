module Color = Color_wrapper;

/** [t] is the type representing creation options for a window */
type t = {
  resizable: bool,
  /**
    If [visible] is true, the window will be visible immediately upon creation.

    If [visible] is false, the window will be invisible, and will require a call
    to Window.show to make it visible. Can be useful to hide transient UI states.
  */
  visible: bool,
  maximized: bool,
  decorated: bool,
  width: int,
  height: int,
  backgroundColor: Color.t,
  vsync: bool,
  icon: option(string),
  menu: option(Revery_Native.Menu.menuInfo),
};

let create =
    (
      ~resizable=true,
      ~visible=true,
      ~maximized=false,
      ~decorated=true,
      ~width=800,
      ~height=600,
      ~backgroundColor=Colors.cornflowerBlue,
      ~vsync=true,
      ~icon=None,
      ~menu=None,
      (),
    ) => {
  resizable,
  visible,
  maximized,
  decorated,
  width,
  height,
  backgroundColor,
  vsync,
  icon,
  menu,
};

let default = create();
