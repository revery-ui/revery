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
  /**
    If [maximized] is true, the window will be expanded immediately upon creation.
   */
  maximized: bool,
  /**
    If [decorated] is true, the window will be framed with the OS-specific adorners.
    */
  decorated: bool,
  /**
    [width] is the initial horizontal size of the [Window]
    */
  width: int,
  /**
    [height] is the initial vertical size of the [Window]
  */
  height: int,
  /**
    [backgroundColor] specifies the initial Color of the [Window]
  */
  backgroundColor: Color.t,
  /**
    If [vsync] is true (default), rendering will be synchronized to the monitor refresh rate.

    This can decrease tearing, but also limits the frame rate to the monito refresh rate.
  */
  vsync: bool,
  /*
      [icon] is an optional path to an icon to show in the window frame.
   */
  icon: option(string),
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
};

let default = create();
