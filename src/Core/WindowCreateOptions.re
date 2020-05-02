/** [t] is the type representing creation options for a window */
type t = {
  resizable: bool,
  /**
    If [visible] is true, the window will be visible immediately upon creation.

    If [visible] is false, the window will be invisible, and will require a call
    to Window.toString to make it visible. Can be useful to hide transient UI states.
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
   [titlebarStyle] sets the appearance of the titlebar. Eventually this will be platform
   independent, but as of right now, Transparent only works on macOS.
    */
  titlebarStyle: WindowStyles.titlebar,
  /**
    [x] is the initial horizontal position of the [Window], either [`Centered]
    or [`Absolute(x)], where [x] is the horizontal pixel coordinate of the left
    edge of the [Window]
    */
  x: [ | `Centered | `Absolute(int)],
  /**
    [y] is the initial horizontal position of the [Window], either [`Centered]
    or [`Absolute(x)], where [y] is the vertical pixel coordinate of the top
    edge of the [Window]
    */
  y: [ | `Centered | `Absolute(int)],
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
  vsync: Vsync.t,
  /*
      [icon] is an optional path to an icon to show in the window frame.
   */
  icon: option(string),
  /*
       [forceScaleFactor] is an optional value to force scaling factor. Scaling affects both the outer window dimensions,
       as well as the scaling of UI elements.
   */
  forceScaleFactor: option(float),
};

let create =
    (
      ~resizable=true,
      ~visible=true,
      ~maximized=false,
      ~decorated=true,
      ~titlebarStyle=WindowStyles.System,
      ~x=`Centered,
      ~y=`Centered,
      ~width=800,
      ~height=600,
      ~backgroundColor=Colors.cornflowerBlue,
      ~vsync=Vsync.Synchronized,
      ~icon=None,
      ~forceScaleFactor=None,
      (),
    ) => {
  resizable,
  visible,
  maximized,
  decorated,
  titlebarStyle,
  x,
  y,
  width,
  height,
  backgroundColor,
  forceScaleFactor,
  vsync,
  icon,
};

let default = create();
