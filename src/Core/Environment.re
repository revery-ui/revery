let isNative =
  switch (Sys.backend_type) {
  | Native => true
  | Bytecode => true
  | _ => false
  };

let webGL = !isNative;

let sleep = (t: Time.t) =>
  /* No-op in JS */
  if (isNative) {
    Unix.sleepf(Time.toFloatSeconds(t));
  };

external yield: unit => unit = "caml_thread_yield";

type os =
  Revery_Native.Environment.os =
    | Unknown
    | Android
    | IOS
    | Browser
    | Mac({
        major: int,
        minor: int,
        bugfix: int,
      })
    | Linux({
        kernel: int,
        major: int,
        minor: int,
        patch: int,
      })
    | Windows({
        major: int,
        minor: int,
        build: int,
      });

let os = {
  webGL ? Browser : Revery_Native.Environment.getOS();
};

let osString =
  switch (os) {
  | Mac({major, minor, bugfix}) =>
    Printf.sprintf("macOS %d.%d.%d", major, minor, bugfix)
  | Linux({kernel, major, minor, patch}) =>
    Printf.sprintf("Linux %d.%d.%d-%d", kernel, major, minor, patch)
  | Windows({major, minor, build}) =>
    Printf.sprintf("Windows %d.%d Build %d", major, minor, build)
  | Android => "Android"
  | Browser => "Browser"
  | IOS => "iOS"
  | Unknown => "Unknown"
  };

let isMac =
  switch (os) {
  | Mac(_) => true
  | _ => false
  };
let isLinux =
  switch (os) {
  | Linux(_) => true
  | _ => false
  };
let isWindows =
  switch (os) {
  | Windows(_) => true
  | _ => false
  };
let isIOS = {
  os == IOS;
};
let isAndroid = {
  os == Android;
};
let isBrowser = {
  os == Browser;
};

module Internal = {
  let addTrailingSlash = dir => {
    let len = String.length(dir);
    if (len == 0) {
      dir;
    } else {
      switch (dir.[len - 1]) {
      | '/' => dir
      | '\\' => dir
      | _ => dir ++ Filename.dir_sep
      };
    };
  };
};

let getExecutingDirectory = () =>
  if (!isNative) {
    "";
  } else {
    let dir =
      switch (os) {
      /* The default strategy of preferring Sys.executable_name seems to only work
       * reliably on Mac.  On Linux, it will return the symlink source instead of
       * the symlink destination - this causes problems when trying to load assets
       * relative to the binary location when symlinked.
       */
      | Mac(_) =>
        switch (
          String.rindex_opt(Sys.executable_name, '/'),
          String.rindex_opt(Sys.executable_name, '\\'),
        ) {
        | (Some(v1), Some(v2)) =>
          String.sub(Sys.executable_name, 0, max(v1, v2))
        | (None, Some(v)) => String.sub(Sys.executable_name, 0, v)
        | (Some(v), None) => String.sub(Sys.executable_name, 0, v)
        | _ => Sys.executable_name
        }
      | _ =>
        let candidatePath = Filename.dirname(Sys.argv[0]) ++ Filename.dir_sep;

        if (Filename.is_relative(candidatePath)) {
          Internal.addTrailingSlash(Sys.getcwd()) ++ candidatePath;
        } else {
          candidatePath;
        };
      };

    /* Check if there is a trailing slash. If not, we need to add one. */
    Internal.addTrailingSlash(dir);
  };

let executingDirectory = getExecutingDirectory();

let getAssetPath = p => {
  isNative
    ? {
      Rench.Path.isAbsolute(p) ? p : executingDirectory ++ p;
    }
    : p;
};

let getWorkingDirectory = () => Sys.getcwd();

let getTempDirectory = () => Filename.get_temp_dir_name();

let getUserLocale = () => Revery_Native.Locale.getUser();
let userLocale = getUserLocale();
