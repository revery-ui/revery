open Reglfw.Glfw;

type performanceFunction = unit => unit;

let nestingLevel = ref(0);

module MemoryAllocations {
    type t = {
        minorWords: int,
        promotedWords: int,
        majorWords: int,
    };

    
    let show = ({minorWords, promotedWords, majorWords}: t) => {
        "| minor: " ++ string_of_int(minorWords) ++ " | major: " ++ string_of_int(majorWords) ++ " | promoted: " ++ string_of_int(promotedWords) ++ " |"
    };
}
let getMemoryAllocations =  (startCounters, endCounters) => {
    let (startMinor, startPromoted, startMajor) = startCounters;   
    let (endMinor, endPromoted, endMajor) = endCounters;

    let ret: MemoryAllocations.t = {
        minorWords: int_of_float(endMinor -. startMinor),
        promotedWords: int_of_float(endPromoted -. startPromoted),
        majorWords: int_of_float(endMajor -. startMajor),
    };
    ret;
};

let bench =
  switch (Sys.getenv_opt("REVERY_DEBUG")) {
  | None => ((_name, f) => f())
  | Some(_) => (
      (name, f) => {
        nestingLevel := nestingLevel^ + 1;
        let startTime = glfwGetTime();
        let startCounters = GarbageCollector.counters();
        f();
        let endTime = glfwGetTime();
        let endCounters = GarbageCollector.counters();
        let allocations = getMemoryAllocations(startCounters, endCounters);
        print_endline(
          String.make(nestingLevel^, '-')
          ++ "[PERF: "
          ++ name
          ++ "] Time: "
          ++ string_of_float((endTime -. startTime) *. 1000.)
          ++ "ms"
          ++ " Memory: "
          ++ MemoryAllocations.show(allocations)
        );

        nestingLevel := nestingLevel^ - 1;
      }
    )
  };
