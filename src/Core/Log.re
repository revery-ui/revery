type severity =
  | Info
  | Error;

type logFunc = (severity, string) => unit;

let _logEvent = Event.create();

let listen = logFunc => {
  Event.subscribe(_logEvent, ((sev, str)) => {logFunc(sev, str)});
};

let info = (category, str) => {
  Event.dispatch(_logEvent, (Info, category ++ ": " ++ str));
};

let error = (category, str) => {
  Event.dispatch(_logEvent, (Error, category ++ ": " ++ str));
};
