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

type os =
  | Windows
  | Mac
  | Linux
  | Browser
  | Unknown;

let os: os;
