type performanceFunction =  unit => unit;

let bench = (name, f) => {
    let startTime = Unix.gettimeofday();
    f();
    let endTime = Unix.gettimeofday();
    print_endline("[PERF: " ++ name ++ "]: " ++ string_of_float((endTime -. startTime) /. 1000.) ++ "ms");
};
