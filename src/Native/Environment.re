external get_os: unit => string = "revery_getOperatingSystem";
let get_os = () =>
  switch (get_os()) {
  | "android" => `Android
  | "ios" => `IOS
  | "linux" => `Linux
  | "mac" => `Mac
  | "windows" => `Windows
  | _ => `Unknown
  };
