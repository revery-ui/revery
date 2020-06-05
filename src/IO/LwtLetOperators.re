let (let.map) = (promise, fn) => Lwt.map(fn, promise);
let (let.mapOk) = (promise, fn) =>
  Lwt.map(
    fun
    | Ok(response) => fn(response)
    | Error(e) => e,
    promise,
  );

let (let.flatMap) = (promise, fn) => Lwt.bind(promise, fn);

let (let.flatMapOk) = (promise, fn) =>
  Lwt.bind(
    promise,
    fun
    | Ok(response) => fn(response)
    | Error(e) => Lwt.return(Error(e)),
  );
