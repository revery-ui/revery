open Reglfw.Glfw;

type performanceFunction = unit => unit;

let nestingLevel = ref(0);

let bench = (name, f) => {
  nestingLevel := nestingLevel^ + 1;
  let startTime = glfwGetTime();
  f();
  let endTime = glfwGetTime();
  print_endline(
    String.make(nestingLevel^, '-')
    ++ "[PERF: "
    ++ name
    ++ "]: "
    ++ string_of_float((endTime -. startTime) /. 1000.)
    ++ "ms",
  );
  nestingLevel := nestingLevel^ - 1;
};
