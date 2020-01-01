type t = {
  title: string,
  body: string,
  onClick: unit => unit,
  mute: bool,
};

let create =
    (~title: string, ~body: string, ~onClick=() => (), ~mute=false, ()) => {
  title,
  body,
  onClick,
  mute,
};

external dispatch: t => unit = "revery_dispatchNotification";
external scheduleFromNow: (int, t) => unit =
  "revery_scheduleNotificationFromNow";
