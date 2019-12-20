type performanceFunction('a) = unit => 'a;

let nestingLevel = ref(0);

module MemoryAllocations = {
  type t = {
    minorWords: int,
    promotedWords: int,
    majorWords: int,
  };

  let toString = ({minorWords, promotedWords, majorWords}: t) => {
    "| minor: "
    ++ string_of_int(minorWords)
    ++ " | major: "
    ++ string_of_int(majorWords)
    ++ " | promoted: "
    ++ string_of_int(promotedWords)
    ++ " |";
  };
};
let getMemoryAllocations = (startCounters, endCounters) => {
  let (startMinor, startPromoted, startMajor) = startCounters;
  let (endMinor, endPromoted, endMajor) = endCounters;

  let ret: MemoryAllocations.t = {
    minorWords: int_of_float(endMinor -. startMinor),
    promotedWords: int_of_float(endPromoted -. startPromoted),
    majorWords: int_of_float(endMajor -. startMajor),
  };
  ret;
};

let isBenchmarking =
  switch (Sys.getenv_opt("REVERY_PERF")) {
  | Some(_) => true
  | None => false
  };

let bench: (string, performanceFunction('a)) => 'a =
  (name, f) =>
    if (isBenchmarking) {
      nestingLevel := nestingLevel^ + 1;
      let startTime = Unix.gettimeofday();
      let startCounters = GarbageCollector.counters();
      Log.perf(String.make(nestingLevel^, '-') ++ "[BEGIN: " ++ name ++ "]");
      let ret = f();
      let endTime = Unix.gettimeofday();
      let endCounters = GarbageCollector.counters();
      let allocations = getMemoryAllocations(startCounters, endCounters);
      Log.perf(
        String.make(nestingLevel^, '-')
        ++ "[END: "
        ++ name
        ++ "] Time: "
        ++ string_of_float((endTime -. startTime) *. 1000.)
        ++ "ms"
        ++ " Memory: "
        ++ MemoryAllocations.toString(allocations),
      );

      nestingLevel := nestingLevel^ - 1;
      ret;
    } else {
      f();
    };
