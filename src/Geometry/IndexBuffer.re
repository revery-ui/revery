open Reglfw;

type t = {
  buffer: Glfw.buffer,
  length: ref(int),
};

let create = () => {
  let buffer = Glfw.glCreateBuffer();

  let length = ref(0);
  let result: t = {buffer, length};
  result;
};

let setData = (ib: t, arr: array(int)) => {
  let {buffer, length} = ib;
  let fArray = Uint16Array.of_array(arr);
  length := Array.length(arr);
  Glfw.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
  Glfw.glBufferData(GL_ELEMENT_ARRAY_BUFFER, fArray, GL_STATIC_DRAW);
};

let draw = (ib: t) => {
  let {buffer, length} = ib;
  Glfw.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
  Glfw.glDrawElements(GL_TRIANGLES, length^, GL_UNSIGNED_SHORT, 0);
};
