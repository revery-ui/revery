/**
  [Environment] provides information about the run-time environment
*/

/** [true] if native / bytecode build, [false] if JSOO build */
let isNative: bool;

/** [true] if JSOO build, [false] otherwise */
let webGL: bool;

/**
  [sleep(t)] sleeps the running thread for the specified time period.

  {b Example usage}: [sleep(Seconds(1.0))]

  {i No-op in JSOO builds.}
*/
let sleep: Time.t => unit;

let yield: unit => unit;

/**
[@deprecated]
*/
let getExecutingDirectory: unit => string;

/**
[executingDirectory] gets the directory where the running executable lives.
*/
let executingDirectory: string;

let getWorkingDirectory: unit => string;

/**
[getAssetPath] resolves a path to an absolute path. If the path is not already
absolute, it is assumed to be relative to the current binary.
*/
let getAssetPath: string => string;

/**
  [getTempDirectory]
  Unix:
  returns the value of the TMPDIR environment variable or "/tmp" if not set.
  Windows:
  returns the value of the TMPDIR environment variable or "." if not set.
*/
let getTempDirectory: unit => string;

type os =
  | Unknown
  | Android
  | IOS
  | Windows
  | Browser
  | Mac(int, int, int)
  | Linux(int, int, int, int);

let os: os;
let osString: string;

let isMac: bool;
let isIOS: bool;
let isWindows: bool;
let isAndroid: bool;
let isLinux: bool;
let isBrowser: bool;

/**
[getUserLocale] returns the current user locale. Note that on some platforms
(including macOS) the locale can change during runtime
*/
let getUserLocale: unit => string;
let userLocale: string;
