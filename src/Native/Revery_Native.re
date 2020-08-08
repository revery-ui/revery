%import
"config.h";

module Dialog = Dialog;
module Environment = Environment;
module Icon = Icon;
module Notification = Notification;
module Shell = Shell;
module Locale = Locale;
module Platform = {
  module Gtk = Gtk;
};

%ifdef
USE_GTK;
let renderLoop = Gtk.renderLoop;
[%%else];
let renderLoop = Sdl2.renderLoop;
[%%endif];

include Initialization;
